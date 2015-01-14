/**
 * @file
 * vuo.leap.hand.sort.distance.z node implementation.
 *
 * @copyright Copyright © 2012–2013 Kosada Incorporated.
 * This code may be modified and distributed under the terms of the MIT License.
 * For more information, see http://vuo.org/license.
 */

#include "node.h"

VuoModuleMetadata({
					  "title" : "Sort Hands by Z Distance",
					  "description" :
						  "<p>Puts the hands in order of nearest to farthest from a target point, \
						  comparing only the distance along the z-axis.</p> \
						  <p>The distance to the target point is calculated from each hand's palm position.</p> \
						  ",
					  "keywords" : [ "organize", "sort", "distance", "nearest", "filter", "shuffle", "point", "z" ],
					  "version" : "1.0.0",
					  "node": {
						  "isInterface" : false
					  }
				  });

typedef struct
{
	int index;
	float value;
} sortable_pointValue;


int compare (const void * a, const void * b)
{
	sortable_pointValue *x = (sortable_pointValue*)a;
	sortable_pointValue *y = (sortable_pointValue*)b;

	return (x->value - y->value);
}

void nodeEvent
(
		VuoInputData(VuoList_VuoLeapHand) hands,
		VuoInputData(VuoPoint3d, {"default":{"x":0, "y":0, "z":0}}) target,
		VuoOutputData(VuoList_VuoLeapHand) sortedHands
)
{
	*sortedHands = VuoListCreate_VuoLeapHand();

	int count = VuoListGetCount_VuoLeapHand(hands);

	sortable_pointValue pointValues[count];

	for(int i = 0; i < count; i++)
		pointValues[i] = (sortable_pointValue){i, fabs(VuoListGetValueAtIndex_VuoLeapHand(hands, i+1).palmPosition.z - target.z)};

	qsort (pointValues, count, sizeof(sortable_pointValue), compare);

	for(int i = 0; i < count; i++)
		VuoListAppendValue_VuoLeapHand(*sortedHands, VuoListGetValueAtIndex_VuoLeapHand(hands, pointValues[i].index+1) );
}
