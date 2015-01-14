/**
 * @file
 * vuo.color.get.rgb node implementation.
 *
 * @copyright Copyright © 2012–2013 Kosada Incorporated.
 * This code may be modified and distributed under the terms of the MIT License.
 * For more information, see http://vuo.org/license.
 */

#include "node.h"

VuoModuleMetadata({
					 "title" : "Get RGB Color Values",
					 "description" :
						"<p>Gives the RGBA (red, blue, green, alpha) components of a color.</p> \
						<p>Red, green, blue, and alpha typically range from 0 to 1 (although numbers outside this range may be used for high dynamic range imaging). \
						An alpha value of 0 is completely transparent, and an alpha value of 1 is completely opaque.</p>",
					 "keywords" : [ "red", "green", "blue", "alpha", "opacity", "transparent", "channel" ],
					 "version" : "1.0.0",
					 "node": {
						 "isInterface" : false
					 }
				 });

void nodeEvent
(
		VuoInputData(VuoColor, {"default":{"r":1,"g":1,"b":1,"a":1}}) color,
		VuoOutputData(VuoReal) red,
		VuoOutputData(VuoReal) green,
		VuoOutputData(VuoReal) blue,
		VuoOutputData(VuoReal) alpha
)
{
	VuoColor_getRGBA(color, red, green, blue, alpha);
}
