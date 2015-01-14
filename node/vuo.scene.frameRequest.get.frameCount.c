/**
 * @file
 * vuo.scene.frameRequest.get.frameCount node implementation.
 *
 * @copyright Copyright © 2012–2013 Kosada Incorporated.
 * This code may be modified and distributed under the terms of the MIT License.
 * For more information, see http://vuo.org/license.
 */

#include "node.h"

VuoModuleMetadata({
					 "title" : "Get Frame Request Count",
					 "description" :
						 "<p>Gives the total number of frames requested by a window so far.</p>",
					 "keywords" : [],
					 "version" : "1.0.0",
					 "node": {
						 "isInterface" : false
					 }
				 });

void nodeEvent
(
		VuoInputData(VuoFrameRequest) frameRequest,
		VuoOutputData(VuoInteger) frameCount
)
{
	*frameCount = frameRequest.frameCount;
}
