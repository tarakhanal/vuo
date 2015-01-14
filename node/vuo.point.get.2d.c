/**
 * @file
 * vuo.point.get.2d node implementation.
 *
 * @copyright Copyright © 2012–2013 Kosada Incorporated.
 * This code may be modified and distributed under the terms of the MIT License.
 * For more information, see http://vuo.org/license.
 */

#include "node.h"

VuoModuleMetadata({
					 "title" : "Get 2D Point Values",
					 "description" :
						"<p>Gives the coordinates of a 2D point.</p>",
					 "keywords" : [ "xy", "cartesian", "euler", "coordinates" ],
					 "version" : "1.0.0",
					 "node": {
						 "isInterface" : false
					 }
				 });

void nodeEvent
(
		VuoInputData(VuoPoint2d, {"default":{"x":0,"y":0}}) point,
		VuoOutputData(VuoReal) x,
		VuoOutputData(VuoReal) y
)
{
	*x = point.x;
	*y = point.y;
}
