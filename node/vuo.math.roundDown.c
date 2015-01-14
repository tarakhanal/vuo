/**
 * @file
 * vuo.math.roundDown node implementation.
 *
 * @copyright Copyright © 2012–2013 Kosada Incorporated.
 * This code may be modified and distributed under the terms of the MIT License.
 * For more information, see http://vuo.org/license.
 */

#include "node.h"
#include "math.h"

VuoModuleMetadata({
					 "title" : "Round Down",
					 "description" :
						"<p>Rounds the number downward to the nearest integer below it.</p>",
					 "keywords" : [ "floor", "near", "close", "approximate", "integer", "whole" ],
					 "version" : "1.0.0",
					 "node": {
						 "isInterface" : false
					 }
				 });

void nodeEvent
(
		VuoInputData(VuoReal, {"default":0.0}) real,
		VuoOutputData(VuoInteger) roundedDownInteger
)
{
	*roundedDownInteger = (VuoInteger)floor(real);
}
