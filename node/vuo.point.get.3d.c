/**
 * @file
 * vuo.point.get.3d node implementation.
 *
 * @copyright Copyright © 2012–2013 Kosada Incorporated.
 * This code may be modified and distributed under the terms of the MIT License.
 * For more information, see http://vuo.org/license.
 */

#include "node.h"

VuoModuleMetadata({
					 "title" : "Get 3D Point Values",
					 "description" :
						"<p>Gives the coordinates of a 3D point.</p>",
					 "keywords" : [ "xyz", "cartesian", "euler", "coordinates" ],
					 "version" : "1.0.0",
					 "node": {
						 "isInterface" : false
					 }
				 });

void nodeEvent
(
		VuoInputData(VuoPoint3d, {"default":{"x":0,"y":0,"z":0}}) point,
		VuoOutputData(VuoReal) x,
		VuoOutputData(VuoReal) y,
		VuoOutputData(VuoReal) z
)
{
	*x = point.x;
	*y = point.y;
	*z = point.z;
}
