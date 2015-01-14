/**
 * @file
 * VuoLeapHand C type definition.
 *
 * @copyright Copyright © 2012–2013 Kosada Incorporated.
 * This code may be modified and distributed under the terms of the MIT License.
 * For more information, see http://vuo.org/license.
 */

#ifndef VUOLEAPHAND_H
#define VUOLEAPHAND_H

#include "VuoInteger.h"
#include "VuoPoint3d.h"
#include "VuoReal.h"

/// @{
#ifndef VuoList_VuoLeapPointable_TYPE_DEFINED
typedef void * VuoList_VuoLeapPointable;
#define VuoList_VuoLeapPointable_TYPE_DEFINED
#endif
/// @}


/**
 * @ingroup VuoTypes
 * @defgroup VuoLeapHand VuoLeapHand
 * Physical characteristics of a detected hand.
 *
 * @{
 */

/**
 * The Hand class reports the physical characteristics of a detected hand.
 */
typedef struct
{
	VuoInteger id;
	VuoPoint3d direction;
	VuoPoint3d palmNormal;
	VuoPoint3d palmPosition;
	VuoPoint3d palmVelocity;
	VuoReal sphereRadius;
	VuoPoint3d sphereCenter;
	VuoList_VuoLeapPointable pointables;

} VuoLeapHand;

VuoLeapHand VuoLeapHand_valueFromJson(struct json_object * js);
struct json_object * VuoLeapHand_jsonFromValue(const VuoLeapHand value);
char * VuoLeapHand_summaryFromValue(const VuoLeapHand value);

VuoLeapHand VuoLeapHand_make(
		VuoInteger id,
		VuoPoint3d direction,
		VuoPoint3d palmNormal,
		VuoPoint3d palmPosition,
		VuoPoint3d palmVelocity,
		VuoReal sphereRadius,
		VuoPoint3d sphereCenter,
		VuoList_VuoLeapPointable pointables);

/// @{
/**
 * Automatically generated function.
 */
VuoLeapHand VuoLeapHand_valueFromString(const char * initializer);
char * VuoLeapHand_stringFromValue(const VuoLeapHand value);
/// @}

/**
 * @}
 */

#endif
