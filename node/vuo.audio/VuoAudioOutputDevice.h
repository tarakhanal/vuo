/**
 * @file
 * VuoAudioOutputDevice C type definition.
 *
 * @copyright Copyright © 2012–2014 Kosada Incorporated.
 * This code may be modified and distributed under the terms of the MIT License.
 * For more information, see http://vuo.org/license.
 */

#ifndef VuoAudioOutputDevice_H
#define VuoAudioOutputDevice_H

/**
 * @ingroup VuoTypes
 * @defgroup VuoAudioOutputDevice VuoAudioOutputDevice
 * Information about an audio output device.
 *
 * @{
 */

#include "VuoInteger.h"
#include "VuoText.h"

/**
 * Information about an audio ouput device.
 */
typedef struct
{
	VuoInteger id;	///< If @c id is non-negative, use the specified device identifier.
	VuoText name;	///< If @c id is negative, use the first device whose name contains @c name.

	VuoInteger channelCount;	///< The number of output channels on this device.
} VuoAudioOutputDevice;

VuoAudioOutputDevice VuoAudioOutputDevice_valueFromJson(struct json_object * js);
struct json_object * VuoAudioOutputDevice_jsonFromValue(const VuoAudioOutputDevice value);
char * VuoAudioOutputDevice_summaryFromValue(const VuoAudioOutputDevice value);

/**
 * Automatically generated function.
 */
///@{
VuoAudioOutputDevice VuoAudioOutputDevice_valueFromString(const char *str);
char * VuoAudioOutputDevice_stringFromValue(const VuoAudioOutputDevice value);
void VuoAudioOutputDevice_retain(VuoAudioOutputDevice value);
void VuoAudioOutputDevice_release(VuoAudioOutputDevice value);
///@}

/**
 * Returns an audio output device with the specified values.
 */
static inline VuoAudioOutputDevice VuoAudioOutputDevice_make(VuoInteger id, VuoText name, VuoInteger channelCount) __attribute__((const));
static inline VuoAudioOutputDevice VuoAudioOutputDevice_make(VuoInteger id, VuoText name, VuoInteger channelCount)
{
	VuoAudioOutputDevice aid = {id,name,channelCount};
	return aid;
}

/**
 * @}
 */

#endif // VuoAudioOutputDevice_H