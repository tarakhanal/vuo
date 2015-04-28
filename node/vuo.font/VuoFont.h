/**
 * @file
 * VuoFont C type definition.
 *
 * @copyright Copyright © 2012–2014 Kosada Incorporated.
 * This code may be modified and distributed under the terms of the MIT License.
 * For more information, see http://vuo.org/license.
 */

#ifndef VUOFONT_H
#define VUOFONT_H

#include "VuoBoolean.h"
#include "VuoColor.h"
#include "VuoHorizontalAlignment.h"
#include "VuoInteger.h"
#include "VuoReal.h"
#include "VuoText.h"

/**
 * @ingroup VuoTypes
 * @defgroup VuoFont VuoFont
 * A font description: family, style, point size, underline.
 *
 * @{
 */

/**
 * A font description: family, style, point size, underline.
 */
typedef struct
{
	VuoText fontName;	///< Unique font machine name.  Includes variants such as bold, italic, and oblique.
	VuoReal pointSize;
	VuoBoolean underline;
	VuoColor color;
	VuoHorizontalAlignment alignment;
	VuoReal characterSpacing;	///< A value of 1.0 is normal character spacing.  Must be >= 0.0.
	VuoReal lineSpacing;	///< A value of 1.0 is normal line spacing.  Must be >= 0.0.
} VuoFont;

VuoFont VuoFont_make(VuoText fontName, VuoReal pointSize, VuoBoolean underline, VuoColor color, VuoHorizontalAlignment alignment, VuoReal characterSpacing, VuoReal lineSpacing);

VuoFont VuoFont_valueFromJson(struct json_object * js);
struct json_object * VuoFont_jsonFromValue(const VuoFont value);
char * VuoFont_summaryFromValue(const VuoFont value);

/// @{
/**
 * Automatically generated function.
 */
VuoFont VuoFont_valueFromString(const char *str);
char * VuoFont_stringFromValue(const VuoFont value);
void VuoFont_retain(VuoFont value);
void VuoFont_release(VuoFont value);
/// @}

/**
 * @}
 */

#endif