/**
 * @file
 * VuoReal implementation.
 *
 * @copyright Copyright © 2012–2018 Kosada Incorporated.
 * This code may be modified and distributed under the terms of the MIT License.
 * For more information, see http://vuo.org/license.
 */

#include "type.h"
#include "VuoInteger.h"
#include "VuoReal.h"
#include "VuoText.h"
#include "VuoList_VuoReal.h"
#include <float.h>
#include <limits.h>

/// @{
#ifdef VUO_COMPILER
VuoModuleMetadata({
					 "title" : "Real",
					 "description" : "A floating-point number.",
					 "keywords" : [ "double", "float", "number" ],
					 "version" : "1.0.0",
					 "dependencies" : [
						"VuoList_VuoReal",
						"VuoInteger",
						"VuoText"
					 ]
				 });
#endif
/// @}

/**
 * @ingroup VuoSceneObject
 * Decodes the JSON object `js` to create a new value.
 */
VuoReal VuoReal_makeFromJson(json_object * js)
{
	if (!js)
		return 0;

	json_type t = json_object_get_type(js);
	if (t == json_type_double)
		return json_object_get_double(js);
	else if (t == json_type_int)
		return json_object_get_int64(js);
	else
	{
		// Use atof() instead of json_object_get_double(),
		// since the latter doesn't support JSON strings with scientific notation
		// or nonnumeric characters following the number.
		const char *s = json_object_get_string(js);
		if (s)
			return atof(s);
		else
			return 0;
	}
}

/**
 * @ingroup VuoReal
 * Encodes @c value as a JSON object.
 */
json_object * VuoReal_getJson(const VuoReal value)
{
	// json spec doesn't support inf or nan by default,
	// but VuoReal_makeFromJson does.
	if( !isfinite(value) )
	{
		if(isnan(value))
			return json_object_new_string("nan");
		else if(value == -INFINITY)
			return json_object_new_string("-inf");
		else
			return json_object_new_string("inf");
	}

	return json_object_new_double(value);
}

/**
 * @ingroup VuoReal
 * Returns a string representation of @c value (either decimal or scientific notation, whichever is shorter).
 */
char * VuoReal_getSummary(const VuoReal value)
{
	return VuoText_format("%g", value);
}

/**
 * Returns the minimum of an array of terms, or 0 if the array is empty.
 */
VuoReal VuoReal_min(VuoReal *terms, unsigned long termsCount, VuoInteger *outputPosition)
{
	if (termsCount == 0)
	{
		*outputPosition = 0;
		return 0;
	}

	VuoReal min = DBL_MAX;
	for (unsigned long i = 0; i < termsCount; ++i)
		if (terms[i] < min)
		{
			min = terms[i];
			*outputPosition = i + 1;
		}

	return min;
}

/**
 * Returns the maximum of an array of terms, or 0 if the array is empty.
 */
VuoReal VuoReal_max(VuoReal *terms, unsigned long termsCount, VuoInteger *outputPosition)
{
	if (termsCount == 0)
	{
		*outputPosition = 0;
		return 0;
	}

	VuoReal max = -DBL_MAX;
	for (unsigned long i = 0; i < termsCount; ++i)
		if (terms[i] > max)
		{
			max = terms[i];
			*outputPosition = i + 1;
		}

	return max;
}

/**
 * Returns the average of the values in the list, or 0 if the list is empty.
 */
VuoReal VuoReal_average(VuoList_VuoReal values)
{
	VuoInteger count = VuoListGetCount_VuoReal(values);
	if (count == 0)
		return 0;

	VuoReal sum = 0;
	for (VuoInteger i = 1; i <= count; ++i)
		sum += VuoListGetValue_VuoReal(values, i);

	return sum/count;
}

/**
 * Returns @a value if it is within the range of @a minimum to @a maximum (exclusive),
 * otherwise a value wrapped with modular arithmetic to be within the range.
 */
VuoReal VuoReal_wrap(VuoReal value, VuoReal minimum, VuoReal maximum)
{
	if (value > maximum)
		return minimum + fmod(value-maximum, maximum-minimum);
	else if (value < minimum)
		return maximum - fmod(minimum-value, maximum-minimum);
	else
		return value;
}

/**
 * Returns a pseudorandom value between `minimum` and `maximum`.
 *
 * @see VuoInteger_random
 */
VuoReal VuoReal_random(const VuoReal minimum, const VuoReal maximum)
{
	return ((VuoReal)VuoInteger_random(0, INT_MAX) / (VuoReal)(INT_MAX)) * (maximum - minimum) + minimum;
}

/**
 * Returns a pseudorandom value between `minimum` and `maximum`.
 *
 * @see VuoInteger_randomWithState
 */
VuoReal VuoReal_randomWithState(unsigned short state[3], const VuoReal minimum, const VuoReal maximum)
{
	return ((VuoReal)VuoInteger_randomWithState(state, 0, INT_MAX) / (VuoReal)(INT_MAX)) * (maximum - minimum) + minimum;
}
