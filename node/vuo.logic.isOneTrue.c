/**
 * @file
 * vuo.logic.areSomeTrue node implementation.
 *
 * @copyright Copyright © 2012–2013 Kosada Incorporated.
 * This code may be modified and distributed under the terms of the MIT License.
 * For more information, see http://vuo.org/license.
 */

#include "node.h"

VuoModuleMetadata({
					 "title" : "Is One True",
					 "description" :
						"<p>Outputs <i>true</i> if exactly one term is <i>true</i>.</p> \
						<p>If there are no terms, outputs <i>false</i>.</p> \
						<p>This node is useful for making something happen if exactly one condition is met. \
						You can connect this node's output port to the `which` input port of a `Select Input` or `Select Output` node.</p>",
					 "keywords" : [ "boolean", "condition", "test", "check", "gate", "xor", "^" ],
					 "version" : "1.0.0",
					 "node": {
						 "isInterface" : false
					 }
				 });

void nodeEvent
(
		VuoInputData(VuoList_VuoBoolean) terms,
		VuoOutputData(VuoBoolean) oneTrue
)
{
	int trueCount = 0;
	unsigned long termsCount = VuoListGetCount_VuoBoolean(terms);
	for (unsigned long i = 1; i <= termsCount; ++i)
		trueCount += VuoListGetValueAtIndex_VuoBoolean(terms, i);
	*oneTrue = (trueCount == 1);
}
