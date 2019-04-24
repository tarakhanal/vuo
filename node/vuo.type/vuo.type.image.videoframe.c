﻿/**
 * @file
 * vuo.type.image.videoframe node implementation.
 *
 * @copyright Copyright © 2012–2018 Kosada Incorporated.
 * This code may be modified and distributed under the terms of the MIT License.
 * For more information, see http://vuo.org/license.
 */

#include "node.h"
#include "../vuo.video/VuoVideoFrame.h"

VuoModuleMetadata({
					 "title" : "Convert Image to Video Frame",
					 "keywords" : [ "movie", "timestamp" ],
					 "version" : "1.0.0",
					 "node": {
					 }
				 });

void nodeEvent
(
		VuoInputData(VuoImage) image,
		VuoOutputData(VuoVideoFrame) videoFrame
)
{
	*videoFrame = VuoVideoFrame_make(image, VuoVideoFrame_NoTimestamp, 0);
}
