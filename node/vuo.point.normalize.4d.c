/**
 * @file
 * vuo.point.normalize.4d node implementation.
 *
 * @copyright Copyright © 2012–2013 Kosada Incorporated.
 * This code may be modified and distributed under the terms of the MIT License.
 * For more information, see http://vuo.org/license.
 */

#include "node.h"

VuoModuleMetadata({
					  "title" : "Normalize 4D Point",
					  "description" :
						  "<p>Outputs a unit vector (length of 1) in the same direction as the input vector.</p>",
					  "keywords" : [ "unit", "vector", "length", "magnitude" ],
					  "version" : "1.0.0",
					  "node": {
						  "isInterface" : false
					  }
				  });

void nodeEvent
(
		VuoInputData(VuoPoint4d, {"default":{"x":0,"y":0,"z":0,"w":0}}) point,
		VuoOutputData(VuoPoint4d) normalizedPoint
)
{
	*normalizedPoint = VuoPoint4d_normalize4d(point);
}
