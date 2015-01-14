/**
 * @file
 * VuoWindow implementation.
 *
 * @copyright Copyright © 2012–2013 Kosada Incorporated.
 * This code may be modified and distributed under the terms of the MIT License.
 * For more information, see http://vuo.org/license.
 */

#import "VuoWindow.h"
#import "VuoWindowApplication.h"
#import "VuoWindowTextInternal.h"
#import "VuoWindowOpenGLInternal.h"

#include "module.h"

#ifdef VUO_COMPILER
VuoModuleMetadata({
					 "title" : "VuoWindow",
					 "dependencies" : [
						 "VuoWindowApplication",
						 "VuoWindowTextInternal",
						 "VuoWindowOpenGLInternal"
					 ]
				 });
#endif


/**
 * Creates a VuoWindowApplication instance (if one doesn't already exist).
 *
 * Must be called from the main thread.
 */
void VuoWindowApplication_init(void)
{
	[NSAutoreleasePool new];
	if (! NSApp)
	{
		// http://stackoverflow.com/a/11010614/238387
		[VuoWindowApplication sharedApplication];
		[NSApp activateIgnoringOtherApps:YES];
		[NSApp run];
	}
}

void VuoWindowText_destroy(VuoWindowText w);

/**
 * Creates and displays a window containing a text edit widget.
 *
 * Creates a new QApplication instance if one did not already exist.
 *
 * Can be called from any thread but the main thread.
 */
VuoWindowText VuoWindowText_make(void)
{
	__block VuoWindowTextInternal *window = NULL;
	dispatch_sync(dispatch_get_main_queue(), ^{
					  VuoWindowApplication_init();
					  window = [[VuoWindowTextInternal alloc] init];
					  [window makeKeyAndOrderFront:NSApp];
				   });
	VuoRegister(window, VuoWindowText_destroy);
	return (VuoWindowText *)window;
}

/**
 * Sets up the window to call the trigger functions when its text is edited.
 *
 * Can be called from any thread but the main thread.
 */
void VuoWindowText_enableTriggers
(
		VuoWindowText w,
		VuoOutputTrigger(typedLine, VuoText),
		VuoOutputTrigger(typedWord, VuoText),
		VuoOutputTrigger(typedCharacter, VuoText)
)
{
	VuoWindowTextInternal *window = (VuoWindowTextInternal *)w;
	dispatch_sync(dispatch_get_main_queue(), ^{
					  [window enableTriggersWithTypedLine:typedLine typedWord:typedWord typedCharacter:typedCharacter];
				  });
}

/**
 * Stops the window from calling trigger functions when its text is edited.
 *
 * Can be called from any thread but the main thread.
 */
void VuoWindowText_disableTriggers(VuoWindowText w)
{
	VuoWindowTextInternal *window = (VuoWindowTextInternal *)w;
	dispatch_sync(dispatch_get_main_queue(), ^{
					  [window disableTriggers];
				  });
}

/**
 * Appends text and a linebreak to the text edit widget in the window.
 *
 * Can be called from any thread.
 */
void VuoWindowText_appendLine(VuoWindowText vw, const char *text)
{
	VuoWindowTextInternal *window = (VuoWindowTextInternal *)vw;
	char *textCopy = strdup(text);  // ... in case the caller frees text before the asynchronous block uses it.
	dispatch_async(dispatch_get_main_queue(), ^{
					   [window appendLine:textCopy];
					   free(textCopy);
				   });
}

/**
 * Closes and deallocates a @c VuoWindowText window.
 *
 * Destroys the current QApplication instance if it no longer contains any widgets.
 *
 * Can be called from any thread but the main thread.
 */
void VuoWindowText_destroy(VuoWindowText vw)
{
	VuoWindowTextInternal *window = (VuoWindowTextInternal *)vw;
	dispatch_sync(dispatch_get_main_queue(), ^{
					  [window close];
					  [window release];
				  });
}


void VuoWindowOpenGl_destroy(VuoWindowOpenGl w);

/**
 * Creates and displays a window containing an OpenGL view.
 *
 * Creates a new QApplication instance if one did not already exist.
 *
 * Can be called from any thread but the main thread.
 */
VuoWindowOpenGl VuoWindowOpenGl_make
(
		void (*initCallback)(void *),
		void (*resizeCallback)(void *, unsigned int, unsigned int),
		void (*drawCallback)(void *),
		void *context
)
{
	__block VuoWindowOpenGLInternal *window = NULL;
	dispatch_sync(dispatch_get_main_queue(), ^{
					  VuoWindowApplication_init();
					  window = [[VuoWindowOpenGLInternal alloc] initWithInitCallback:initCallback resizeCallback:resizeCallback drawCallback:drawCallback drawContext:context];
					  [window makeKeyAndOrderFront:nil];
				  });
	VuoRegister(window, VuoWindowOpenGl_destroy);
	return (VuoWindowOpenGl *)window;
}

/**
 * Sets up the window to call the trigger functions when events occur.
 *
 * Can be called from any thread but the main thread.
 */
void VuoWindowOpenGl_enableTriggers
(
		VuoWindowOpenGl w,
		VuoOutputTrigger(requestedFrame, VuoFrameRequest),
		VuoOutputTrigger(movedMouseTo, VuoPoint2d),
		VuoOutputTrigger(scrolledMouse, VuoPoint2d),
		VuoOutputTrigger(usedMouseButton, VuoMouseButtonAction)
)
{
	VuoWindowOpenGLInternal *window = (VuoWindowOpenGLInternal *)w;
	dispatch_sync(dispatch_get_main_queue(), ^{
					  [window enableTriggersWithRequestedFrame:requestedFrame
					  movedMouseTo:movedMouseTo
					  scrolledMouse:scrolledMouse
					  usedMouseButton:usedMouseButton
					  ];
				  });
}

/**
 * Stops the window from calling trigger functions when events occur.
 *
 * Can be called from any thread but the main thread.
 */
void VuoWindowOpenGl_disableTriggers(VuoWindowOpenGl w)
{
	VuoWindowOpenGLInternal *window = (VuoWindowOpenGLInternal *)w;
	dispatch_sync(dispatch_get_main_queue(), ^{
					  [window disableTriggers];
				  });
}

/**
 * Schedules the specified window to be redrawn.
 *
 * Can be called from any thread.
 */
void VuoWindowOpenGl_redraw(VuoWindowOpenGl w)
{
	VuoWindowOpenGLInternal *window = (VuoWindowOpenGLInternal *)w;
	dispatch_async(dispatch_get_main_queue(), ^{
					   [window scheduleRedraw];
				   });
}

/**
 * Sets the window's current and preferred aspect ratio.
 *
 * If necessary, the window is resized to match the specified aspect ratio.
 * After calling this method, when the user resizes the window, its height is adjusted to match the specified aspect ratio.
 */
void VuoWindowOpenGl_setAspectRatio(VuoWindowOpenGl w, unsigned int pixelsWide, unsigned int pixelsHigh)
{
	VuoWindowOpenGLInternal *window = (VuoWindowOpenGLInternal *)w;
	dispatch_async(dispatch_get_main_queue(), ^{
					   [window setAspectRatioToWidth:pixelsWide height:pixelsHigh];
				   });
}

/**
 * Closes and deallocates a @c VuoWindowOpenGl window.
 *
 * Destroys the current QApplication instance if it no longer contains any widgets.
 *
 * Can be called from any thread but the main thread.
 */
void VuoWindowOpenGl_destroy(VuoWindowOpenGl vw)
{
	VuoWindowOpenGLInternal *window = (VuoWindowOpenGLInternal *)vw;
	dispatch_sync(dispatch_get_main_queue(), ^{
					  [window close];
					  [window release];
				  });
}
