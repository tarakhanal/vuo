/**
 * @file
 * VuoCompilerModule interface.
 *
 * @copyright Copyright © 2012–2013 Kosada Incorporated.
 * This interface description may be modified and distributed under the terms of the GNU Lesser General Public License (LGPL) version 2 or later.
 * For more information, see http://vuo.org/license.
 */

#ifndef VUOCOMPILERMODULE_HH
#define VUOCOMPILERMODULE_HH

#include "VuoBaseDetail.hh"
#include "VuoModule.hh"
#include "VuoCompilerTargetSet.hh"

#include "VuoCompilerBitcodeParser.hh"

/**
 * A node class or type defined in an LLVM module.
 *
 * This class would be the compiler detail class for @c VuoModule, except that the inheritance doesn't work out.
 */
class VuoCompilerModule
{
private:
	VuoCompilerTargetSet compatibleTargets;  ///< The set of targets with which this module is compatible.

	static bool isModule(Module *module, string moduleKey);

protected:
	struct json_object *moduleDetails;  ///< This module's metadata, found in the argument to @c VuoModuleMetadata in the module definition.
	vector<string> dependencies;  ///< The dependencies found in this module's metadata
	Module *module;  ///< The LLVM module that defines this node class or type
	VuoModule *base;  ///< The (pseudo) base for this (pseudo) compiler detail class
	VuoCompilerBitcodeParser *parser;  ///< The parser of the LLVM module

	VuoCompilerModule(VuoModule *base, Module *module);
	virtual ~VuoCompilerModule();

	virtual void parse(void);
	virtual void parseMetadata(void);
	string parseString(json_object *o, string key);
	int parseInt(json_object *o, string key);
	bool parseBool(json_object *o, string key);
	vector<string> parseArrayOfStrings(json_object *o, string key);
	virtual set<string> globalsToRename(void);
	string nameForGlobal(string genericGlobalVarOrFuncName);
	static string nameForGlobal(string nameBeforeCompilation, string moduleKey);
	static bool hasOriginalOrMangledGlobal(string nameBeforeCompilation, Module *module, string moduleKey);
	void renameGlobalVarsAndFuncs(void);

	friend class TestVuoCompilerModule;

public:
	static VuoCompilerModule * newModule(string moduleKey, Module *module);

	VuoCompilerTargetSet parseTargetSet(json_object *o, string key);
	VuoCompilerTargetSet::MacVersion parseMacVersion(string version);
	static Function * declareFunctionInModule(Module *module, Function *function);
	vector<string> getDependencies(void);
	VuoCompilerTargetSet getCompatibleTargets(void);
	Module * getModule(void);
};

#endif // VUOCOMPILERMODULE_HH
