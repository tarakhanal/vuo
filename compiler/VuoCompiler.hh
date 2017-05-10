﻿/**
 * @file
 * VuoCompiler interface.
 *
 * @copyright Copyright © 2012–2016 Kosada Incorporated.
 * This interface description may be modified and distributed under the terms of the GNU Lesser General Public License (LGPL) version 2 or later.
 * For more information, see http://vuo.org/license.
 */

#ifndef VUOCOMPILER_H
#define VUOCOMPILER_H

#include "VuoFileUtilities.hh"

class VuoCompilerComposition;
class VuoCompilerException;
class VuoCompilerGenericType;
class VuoCompilerModule;
class VuoCompilerNodeClass;
class VuoCompilerType;
class VuoNode;
class VuoNodeClass;
class VuoNodeSet;
class VuoPublishedPort;
class VuoRunner;


/**
 * This class compiles node classes, types, and library modules. It compiles and links compositions.
 *
 * Node classes, types, library modules, and compositions are all compiled to [LLVM](http://llvm.org/)
 * bitcode. LLVM bitcode is one of the file formats used by the [Clang](http://clang.llvm.org/) compiler.
 *
 * When a composition is linked, the compiled composition and all of its node classes, types, library modules,
 * and other dependencies are combined to create one of the following: an executable, a dynamic library, or
 * or separate dynamic libraries for the composition and its resources.
 *
 * All VuoCompiler instances share a basic set of node classes, types, library modules, and other dependencies.
 * Each VuoCompiler instance can add its own dependencies on top of that basic set.
 *
 * The easiest way to compile, link, and run a composition is to use the VuoCompiler factory methods
 * to create a VuoRunner that's ready to run the composition:
 *
 *    - VuoCompiler::newSeparateProcessRunnerFromCompositionFile()
 *    - VuoCompiler::newSeparateProcessRunnerFromCompositionString()
 *    - VuoCompiler::newCurrentProcessRunnerFromCompositionFile()
 *    - VuoCompiler::newCurrentProcessRunnerFromCompositionString()
 *
 * When building a composition, this class optimizes the build in different ways depending on the functions,
 * and in some cases the arguments, that you use. Some functions for linking a composition accept an
 * Optimization argument. The rest default to the "faster build" optimization (Optimization_FastBuild).
 * The "faster build" optimization works by caching a library on the user's filesystem and reusing it
 * each time a composition is linked. The caching takes a while, and may delay the building of the first
 * composition. However, if you call prepareForFastBuild(), the caching can be done in advance.
 *
 * If you plan to distribute an executable or dynamic library built from a composition, use either
 * linkCompositionToCreateExecutable() or linkCompositionToCreateDynamicLibrary(), and choose the
 * "smaller binary" optimization. If building an executable, pass an rpath (such as "@loader_path/../Frameworks")
 * to enable the executable to find Vuo.framework. Since the executable or dynamic library links to
 * Vuo.framework, you need to distribute it along with the build composition.
 *
 * @only64Bit
 *
 * @see DevelopingApplications
 */
class VuoCompiler
{
public:
	/**
	 * Options for optimizations when building a composition.
	 */
	enum Optimization
	{
		Optimization_FastBuild,  ///< Optimize for a fast build, at the expense of a binary that relies on a large cache library.
		Optimization_FastBuildExistingCache,  ///< Like Optimization_FastBuild, but skips ensuring that the cache library is up to date.
		Optimization_SmallBinary  ///< Optimize for a small self-contained binary, at the expense of the build taking longer.
	};

	/**
	 * A set of node classes, types, library modules, and search paths for other dependencies.
	 *
	 * This is public only so that the static variable VuoCompiler::sharedEnvironment can be defined.
	 */
	class Environment
	{
	private:
		bool isOwner;  ///< True if this instance is responsible for deleting the modules and such.
		map<string, VuoCompilerNodeClass *> nodeClasses;  ///< Node classes loaded, plus specialized node classes generated by the compiler.
		map<string, VuoCompilerType *> types;  ///< Types loaded.
		set<VuoCompilerGenericType *> genericTypes;  ///< Generic types generated by the compiler for node classes.
		map<string, VuoNodeSet *> nodeSetForName;  ///< Node sets loaded.
		map<string, VuoCompilerModule *> libraryModules;  ///< Library modules loaded.
		map<string, bool> isModuleSearchPathLoaded;  ///< Tracks whether modules have been loaded from each of moduleSearchPaths.
		map<string, map<string, VuoFileUtilities::File *> > moduleFilesAtSearchPath;  ///< The module files seen (but not necessarily loaded) from moduleSearchPaths.
		vector<string> moduleSearchPaths;  ///< Search paths for node classes, types, and library modules.
		vector<string> headerSearchPaths;  ///< Search paths for header/include files.
		vector<string> librarySearchPaths;  ///< Search paths for libraries (other than Vuo library modules).
		vector<string> frameworkSearchPaths;  ///< Search paths for frameworks.

		set<VuoFileUtilities::File *> listModules(const string &path);
		VuoCompilerModule * loadModule(VuoFileUtilities::File *moduleFile, const string &searchPath);
		void reifyPortTypes(Environment *outerEnvironment);

	public:
		Environment(bool isOwner);
		~Environment(void);
		map<string, VuoCompilerNodeClass *> getNodeClasses(void);
		VuoCompilerNodeClass * getNodeClass(const string &moduleKey);
		map<string, VuoCompilerType *> getTypes(void);
		VuoCompilerType * getType(const string &moduleKey);
		map<string, VuoCompilerModule *> getLibraryModules(void);
		VuoCompilerModule * getLibraryModule(const string &moduleKey);
		VuoNodeSet * findNodeSet(const string &name);
		void addModuleSearchPath(const string &path);
		vector<string> getModuleSearchPaths(void);
		void addHeaderSearchPath(const string &path);
		vector<string> getHeaderSearchPaths(void);
		void addLibrarySearchPath(const string &path);
		vector<string> getLibrarySearchPaths(void);
		void addFrameworkSearchPath(const string &path);
		vector<string> getFrameworkSearchPaths(void);
		void loadModulesIntoCombinedEnvironment(Environment *myEnvironment, Environment *sharedEnvironment, bool shouldLoadAllModules, set<string> &modulesToLoad);
		void addNodeClassToCombinedEnvironment(VuoCompilerNodeClass *nodeClass, Environment *myEnvironment, Environment *sharedEnvironment);
		void removeNodeClassFromCombinedEnvironment(string nodeClassName, Environment *myEnvironment, Environment *sharedEnvironment);
		void updateCombinedEnvironment(Environment *myEnvironment, Environment *sharedEnvironment);
		vector<string> getBuiltInModuleSearchPaths(void);
		vector<string> getBuiltInLibrarySearchPaths(void);
		void addSearchPathsToSharedEnvironment(void);
		void addCachedModulePathToSharedEnvironment(string moduleFileName);
		void removeCachedModulePathFromSharedEnvironment(string moduleFileName);
	};

private:
	static bool isLlvmInitialized;
	static dispatch_queue_t llvmQueue;  ///< Synchronizes access to LLVM's global context. Don't call environmentQueue from this queue.
	static dispatch_queue_t environmentQueue;  ///< Synchronizes access to the Environment data members. It's OK to call llvmQueue from this queue.
	static dispatch_queue_t cachedResourcesQueue;
	static const int NUM_CACHES;
	static bool hasTriedCachedResources[2];
	static string cachedResourceDylib[2];
	static set<string> cachedResources[2];
	static set<string> encounteredPremiumModules;
	static Environment *sharedEnvironment;
	Environment myEnvironment;
	Environment combinedEnvironment;
	map<string, set<string> > compatibleDependenciesCache;
	map<string, set<string> > incompatibleDependenciesCache;
	dispatch_queue_t dependenciesCacheQueue;  ///< Synchronizes access to compatibleDependenciesCache and incompatibleDependenciesCache.
	map<string, set<string> > compositionPathsAtSearchPath;
	static bool hasCleanedCompiledSubcompositionDir;
	static dispatch_queue_t hasCleanedCompiledSubcompositionDirQueue;  ///< Synchronizes access to hasCleanedCompiledSubcompositionDir.
	bool shouldLoadAllModules;
	set<string> modulesToLoad;
	vector<string> subcompositionsBeingCompiled;
	set<string> subcompositionModules;
	dispatch_queue_t modulesToLoadQueue;  ///< Synchronizes access to modulesToLoad and related variables.
	map<string, VuoCompilerException> errorsLoadingModules;
	llvm::sys::Path clangPath;
	string telemetry;
	string target;
	bool isVerbose;

	static Environment * getSharedEnvironment(void);
	set<string> listCompositions(const string &path);
	void loadModulesIfNeeded(void);
	void reifyGenericPortTypes(VuoCompilerComposition *composition);
	void reifyGenericPortTypes(VuoNode *node);
	bool compileSubcompositionIfNeeded(string compositionPath);
	void uninstallSubcomposition(string nodeClassName);
	void linkCompositionToCreateExecutableOrDynamicLibrary(string compiledCompositionPath, string linkedCompositionPath, Optimization optimization, bool isDylib, bool isApp=false, string rPath="");
	set<string> getDependenciesForComposition(const string &compiledCompositionPath);
	map<string, int> getDependenciesRecursivelyWithCache(const string &dependency, const vector<string> &ancestorsVisited);
	void getLinkerInputs(const set<string> &dependencies, Optimization optimization, set<Module *> &modules, set<string> &libraries, set<string> &frameworks);
	void getCachedResources(bool shouldUseExistingCache=false);
	void getCachedResourcesThreadUnsafe(bool shouldUseExistingCache=false);
	void link(string outputPath, const set<Module *> &modules, const set<string> &libraries, const set<string> &frameworks, bool isDylib, string rPath="");
	Module * readModuleFromC(string inputPath);
	Module * readModuleFromC(string inputPath, const vector<string> &extraArgs);
	static Module * readModuleFromBitcode(VuoFileUtilities::File *inputFile);
	static Module * readModuleFromBitcodeData(char *inputData, size_t inputDataBytes, string &error);
	static bool writeModuleToBitcode(Module *module, string outputPath);
	static void setTargetForModule(Module *module, string target = "");
	VuoCompilerModule * getModule(const string &id);
	static vector<string> getCoreVuoDependencies(void);
	static string getRuntimeMainDependency(void);
	static string getRuntimeDependency(void);
	static string getApplicationDependency(void);
	static string getModuleNameForPath(string path);
	void addModuleSearchPath(string path);
	llvm::sys::Path getClangPath(void);
	void setClangPath(const string &clangPath);
	string getCompositionStubPath(void);
	static string getCachedModulesPath(void);

	friend class TestVuoCompiler;		///< TestVuoCompiler needs to add a search path for its own private nodes/types.
	friend class TestCompositionExecution;	///< TestCompositionExecution needs to add a search path for its own private nodes.
	friend class TestVuoCompilerModule;
	friend class TestVuoCompilerBitcodeGenerator;
	friend class TestCompilingAndLinking;
	friend class TestSubcompositions;

public:
	VuoCompiler(void);
	~VuoCompiler(void);
	void setLicense(string licenseContent, string licenseSignature);
	void loadStoredLicense(bool showLicenseWarning);
	void loadStoredLicense(void);
	static Module * readModuleFromBitcode(string inputPath);
	static void deleteModule(Module *module);
	map<string, VuoCompilerException> flushErrorsLoadingModules(void);
	static set<string> getEncounteredPremiumModules();
	void compileComposition(VuoCompilerComposition *composition, string outputPath, bool isTopLevelComposition=true, bool isLiveCodeable=true);
	void compileComposition(string inputPath, string outputPath, bool isTopLevelComposition, bool isLiveCodeable);
	void compileComposition(string inputPath, string outputPath);
	void compileCompositionString(const string &compositionString, string outputPath, bool isTopLevelComposition=true, bool isLiveCodeable=true);
	VuoCompilerNodeClass * installSubcomposition(string compositionPath);
	void compileModule(string inputPath, string outputPath);
	void compileModule(string inputPath, string outputPath, const vector<string> &includeDirs);
	void linkCompositionToCreateExecutable(string inputPath, string outputPath, Optimization optimization, bool isApp=false, string rPath="");
	void linkCompositionToCreateDynamicLibrary(string inputPath, string outputPath, Optimization optimization);
	void linkCompositionToCreateDynamicLibraries(string compiledCompositionPath, string linkedCompositionPath, string &newLinkedResourcePath, vector<string> &alreadyLinkedResourcePaths, set<string> &alreadyLinkedResources);
	void prepareForFastBuild(void);
	void setLoadAllModules(bool shouldLoadAllModules);
	void getDependenciesRecursively(const string &dependency, set<string> &compatibleDependencies, set<string> &incompatibleDependencies);
	VuoNode * createNode(VuoCompilerNodeClass *nodeClass, string title="", double x=0, double y=0);
	VuoNode * createPublishedInputNode(vector<VuoPublishedPort *> publishedInputPorts);
	VuoNode * createPublishedOutputNode(vector<VuoPublishedPort *> publishedOutputPorts);
	VuoCompilerNodeClass * getNodeClass(const string &id);
	map<string, VuoCompilerNodeClass *> getNodeClasses(void);
	VuoCompilerType * getType(const string &id);
	map<string, VuoCompilerType *> getTypes(void);
	VuoNodeSet * getNodeSetForName(const string &name);
	void listNodeClasses(const string &format = "");
	void addHeaderSearchPath(const string &path);
	void addLibrarySearchPath(const string &path);
	void addFrameworkSearchPath(const string &path);
	void setTelemetry(const string &telemetry);
	void setTarget(const string &target);
	void setVerbose(bool isVerbose);
	string getCompositionLoaderPath(void);
	void print(void);

	static VuoRunner * newSeparateProcessRunnerFromCompositionFile(string compositionFilePath);
	static VuoRunner * newSeparateProcessRunnerFromCompositionString(string composition, string workingDirectory);
	static VuoRunner * newCurrentProcessRunnerFromCompositionFile(string compositionFilePath);
	static VuoRunner * newCurrentProcessRunnerFromCompositionString(string composition, string workingDirectory);
};


#endif
