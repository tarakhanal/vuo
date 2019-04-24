/**
 * @file
 * VuoApp implementation.
 *
 * @copyright Copyright © 2012–2018 Kosada Incorporated.
 * This code may be modified and distributed under the terms of the MIT License.
 * For more information, see http://vuo.org/license.
 */

#include "module.h"

#include "VuoApp.h"

/// Disable NS_RETURNS_INNER_POINTER (new in Mac OS 10.10's system headers), since Clang 3.2 doesn't support it.
/// https://b33p.net/kosada/node/9140
#ifndef NS_RETURNS_INNER_POINTER
#define NS_RETURNS_INNER_POINTER
#endif
#import <AppKit/AppKit.h>
#undef NS_RETURNS_INNER_POINTER

#include <dlfcn.h>
#include <pthread.h>
#include <libproc.h>
#include <mach-o/dyld.h>
#import <libgen.h>
#import <dirent.h>

#include "VuoCompositionState.h"
#include "VuoEventLoop.h"
#import "VuoAppDelegate.h"

/**
 * Is the current thread the main thread?
 */
bool VuoApp_isMainThread(void)
{
	static void **VuoApp_mainThread;
	static dispatch_once_t once = 0;
	dispatch_once(&once, ^{
		VuoApp_mainThread = (void **)dlsym(RTLD_SELF, "VuoApp_mainThread");
		if (!VuoApp_mainThread)
			VuoApp_mainThread = (void **)dlsym(RTLD_DEFAULT, "VuoApp_mainThread");

		if (!VuoApp_mainThread)
		{
			VUserLog("Error: Couldn't find VuoApp_mainThread.");
			exit(1);
		}

		if (!*VuoApp_mainThread)
		{
			VUserLog("Error: VuoApp_mainThread isn't set.");
			exit(1);
		}
	});

	return *VuoApp_mainThread == (void *)pthread_self();
}

/**
 * Executes the specified block on the main thread, then returns.
 *
 * Can be called from any thread, including the main thread (avoids deadlock).
 *
 * Don't call this from `__attribute__((constructor))` functions,
 * since this function depends on initialization
 * which might not happen before your constructor is called.
 */
void VuoApp_executeOnMainThread(void (^block)(void))
{
	if (VuoApp_isMainThread())
		block();
	else
	{
		VUOLOG_PROFILE_BEGIN(mainQueue);
		dispatch_sync(dispatch_get_main_queue(), ^{
			VUOLOG_PROFILE_END(mainQueue);
			block();
		});
	}
}

/**
 * Returns the composition's name.
 *
 * If the composition is running in Vuo Editor (via VuoCompositionLoader),
 * it extracts the name from the most-recently-loaded dylib's path.
 * If the composition has been saved, this matches the composition's source filename.
 * If not, it returns the text "Vuo Composition".
 *
 * If the composition is running via VuoRunner (but not via VuoCompositionLoader),
 * it returns the name of the VuoRunner process.
 *
 * If the composition is running standalone, it tries the following Info.plist keys:
 *
 *    - CFBundleDisplayName
 *    - CFBundleName
 *    - CFBundleExecutable
 *
 * If none of those keys exist, it uses the composition executable's filename.
 *
 * The caller is responsible for freeing the returned string.
 */
char *VuoApp_getName(void)
{
	char **dylibPath = (char **)dlsym(RTLD_SELF, "VuoApp_dylibPath");
	if (!dylibPath)
		dylibPath = (char **)dlsym(RTLD_DEFAULT, "VuoApp_dylibPath");
	if (dylibPath)
		if (strncmp(*dylibPath, "/tmp/", 5) == 0)
		{
			char *name = strdup(*dylibPath + 5);
			name[strlen(name) - strlen("-XXXXXX.dylib")] = 0;

			if (strcmp(name, "VuoComposition") == 0)
			{
				free(name);
				return strdup("Vuo Composition");
			}

			return name;
		}

	pid_t runnerPid = VuoGetRunnerPid();
	if (runnerPid > 0)
	{
		char *runnerName = (char *)malloc(2*MAXCOMLEN);
		proc_name(runnerPid, runnerName, 2*MAXCOMLEN);
		return runnerName;
	}

	NSBundle *mainBundle = [NSBundle mainBundle];
	NSString *name = [mainBundle objectForInfoDictionaryKey:@"CFBundleDisplayName"];
	if (!name)
		name = [mainBundle objectForInfoDictionaryKey:@"CFBundleName"];
	if (!name)
		name = [mainBundle objectForInfoDictionaryKey:@"CFBundleExecutable"];
	if (!name)
		name = [[[mainBundle executablePath] stringByDeletingPathExtension] lastPathComponent];

	if (name)
		return strdup([name UTF8String]);
	else
		return strdup("");
}

/**
 * Returns the path of the folder containing `Vuo.framework`.
 *
 * See also @ref VuoFileUtilities::getVuoFrameworkPath.
 */
const char *VuoApp_getVuoFrameworkPath(void)
{
	static char frameworkPath[PATH_MAX+1] = "";
	static dispatch_once_t once = 0;
	dispatch_once(&once, ^{
		for(unsigned int i=0; i<_dyld_image_count(); ++i)
		{
			const char *dylibPath = _dyld_get_image_name(i);
			char *pos;
			if ( (pos = strstr(dylibPath, "/Vuo.framework/")) )
			{
				strncpy(frameworkPath, dylibPath, pos-dylibPath);
				break;
			}
		}
	});
	return frameworkPath;
}

/**
 * Helper for @ref VuoApp_init.
 *
 * @threadMain
 */
static void VuoApp_initNSApplication(void)
{
	NSAutoreleasePool *pool = [NSAutoreleasePool new];

	// http://stackoverflow.com/a/11010614/238387
	NSApplication *app = [NSApplication sharedApplication];

	if (![app delegate])
		[app setDelegate:[VuoAppDelegate new]];

	// When there's no host app present,
	// create the default menu with the About and Quit menu items,
	// to be overridden if/when any windows get focus.
	VuoApp_setMenuItems(NULL);

	// Show the app in the dock (since non-NIB apps are hidden by default).
	[app setActivationPolicy:NSApplicationActivationPolicyRegular];

	// Stop bouncing in the dock.
	[app finishLaunching];

	VuoEventLoop_switchToAppMode();

	[pool drain];
}

#ifndef DOXYGEN
void VuoApp_fini(void);
#endif

/**
 * Creates an NSApplication instance (if one doesn't already exist).
 *
 * This causes the process's icon to appear in the dock.
 *
 * VuoWindow methods call this automatically as needed,
 * so you only need to call this if you need an NSApplication without using VuoWindow
 * (like, for example, @ref VuoAudioFile does).
 *
 * @threadAny
 */
void VuoApp_init(void)
{
	if (NSApp)
		return;

	static dispatch_once_t once = 0;
	dispatch_once(&once, ^{
		VuoApp_executeOnMainThread(^{
			VuoApp_initNSApplication();
		});
		VuoAddCompositionFiniCallback(VuoApp_fini);
	});
}

/**
 * Helper for @ref VuoApp_fini.
 *
 * @threadMain
 */
static void VuoApp_finiWindows(uint64_t compositionUid)
{
	// Stop any window recordings currently in progress.
	// This prompts the user for the save destination,
	// so make sure these complete before shutting the composition down.
	SEL stopRecording = @selector(stopRecording);
	SEL compositionUidSel = @selector(compositionUid);
	for (NSWindow *window in [NSApp windows])
		if ([window respondsToSelector:stopRecording]
		 && [window respondsToSelector:compositionUidSel]
		 && (uint64_t)[window performSelector:compositionUidSel] == compositionUid)
			[window performSelector:stopRecording];

	// Avoid leaving menubar remnants behind.
	// https://b33p.net/kosada/node/13384
	[NSApp hide:nil];
}

/**
 * Cleanly shuts the application down.
 *
 * Should only be called from within @ref vuoStopComposition().
 *
 * @threadAny
 */
void VuoApp_fini(void)
{
	if (!NSApp)
		return;

	void *compositionState = vuoCopyCompositionStateFromThreadLocalStorage();
	uint64_t compositionUid = vuoGetCompositionUniqueIdentifier(compositionState);
	free(compositionState);

	if (VuoApp_isMainThread())
		VuoApp_finiWindows(compositionUid);
	else
	{
		VUOLOG_PROFILE_BEGIN(mainQueue);
		dispatch_sync(dispatch_get_main_queue(), ^{
			VUOLOG_PROFILE_END(mainQueue);
			VuoApp_finiWindows(compositionUid);
		});
	}
}
