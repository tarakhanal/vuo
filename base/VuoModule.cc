/**
 * @file
 * VuoModule implementation.
 *
 * @copyright Copyright © 2012–2013 Kosada Incorporated.
 * This code may be modified and distributed under the terms of the GNU Lesser General Public License (LGPL) version 2 or later.
 * For more information, see http://vuo.org/license.
 */

#include "VuoModule.hh"

/**
 * Creates a module.
 *
 * @param moduleKey A unique name for this module.
 */
VuoModule::VuoModule(string moduleKey)
{
	this->moduleKey = moduleKey;
}

/**
 * Returns this module's unique name.
 */
string VuoModule::getModuleKey(void)
{
	return moduleKey;
}

/**
 * Returns the default title for instances of this module, as specified by the .vuonode/.bc implementation.  Needn't be unique.
 *
 * UTF-8.
 *
 * @eg{Less Than}
 *
 * @see VuoModuleMetadata
 */
string VuoModule::getDefaultTitle(void)
{
	return defaultTitle;
}

/**
 * Sets the default title for instances of this module.  Needn't be unique.
 *
 * UTF-8.
 *
 * @eg{Less Than}
 *
 * @see VuoModuleMetadata
 */
void VuoModule::setDefaultTitle(string defaultTitle)
{
	this->defaultTitle = defaultTitle;
}

/**
 * Returns the description of this module provided as documentation.
 *
 * @see VuoModuleMetadata
 */
string VuoModule::getDescription(void)
{
	return description;
}

/**
 * Sets the description of this module provided as documentation.
 *
 * @see VuoModuleMetadata
 */
void VuoModule::setDescription(string description)
{
	this->description = description;
}

/**
 * Returns the module's version, in Semantic Versioning format.
 *
 * @see VuoModuleMetadata
 */
string VuoModule::getVersion(void)
{
	return version;
}

/**
 * Sets the module's version, in Semantic Versioning format.
 *
 * @see VuoModuleMetadata
 */
void VuoModule::setVersion(string version)
{
	this->version = version;
}

/**
 * Returns a list of the module's keywords.
 *
 * @see VuoModuleMetadata
 */
vector<string> VuoModule::getKeywords(void)
{
	return keywords;
}

/**
 * Sets the module's keywords.
 *
 * @see VuoModuleMetadata
 */
void VuoModule::setKeywords(vector<string> keywords)
{
	this->keywords = keywords;
}
