/**
 * @file
 * vuo.point.subtract.3d node implementation.
 *
 * @copyright Copyright © 2012–2013 Kosada Incorporated.
 * This code may be modified and distributed under the terms of the MIT License.
 * For more information, see http://vuo.org/license.
 */

#include "node.h"

VuoModuleMetadata({
					  "title" : "Subtract 3D Points",
					  "description" :
						  "<p>Calculates `a` minus `b`.</p>",
					  "keywords" : [ "difference", "minus", "-", "vector" ],
					  "version" : "1.0.0",
					  "node" : {
						  "isInterface" : false
					  }
				  });

void nodeEvent
(
		VuoInputData(VuoPoint3d, {"default":{"x":0,"y":0,"z":0}}) a,
		VuoInputData(VuoPoint3d, {"default":{"x":0,"y":0,"z":0}}) b,
		VuoOutputData(VuoPoint3d) difference
)
{
	*difference = VuoPoint3d_subtract(a, b);
}
