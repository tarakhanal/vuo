/**
 * @file
 * vuo.type.integer.point2d node implementation.
 *
 * @copyright Copyright © 2012–2013 Kosada Incorporated.
 * This code may be modified and distributed under the terms of the MIT License.
 * For more information, see http://vuo.org/license.
 */

#include "node.h"

VuoModuleMetadata({
					 "title" : "Convert Integer to 2D Point",
					 "description" :
						 "<p>Creates a 2D point with coordinates (0,y).</p> \
						 <p>This node is useful as a type converter to connect ports with different data types.</p>",
					 "keywords" : [ ],
					 "version" : "1.0.0",
					 "node": {
						 "isInterface" : false
					 }
				 });

void nodeEvent
(
		VuoInputData(VuoInteger, {"default":0}) y,
		VuoOutputData(VuoPoint2d) point2d
)
{
	*point2d = VuoPoint2d_make(0, y);
}
