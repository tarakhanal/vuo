/**
 * @file
 * VuoCglPixelFormat interface.
 *
 * @copyright Copyright © 2012–2018 Kosada Incorporated.
 * This code may be modified and distributed under the terms of the MIT License.
 * For more information, see http://vuo.org/license.
 */

#ifdef __cplusplus
extern "C"
{
#endif

#include <OpenGL/CGLTypes.h>

char *VuoCglPixelFormat_getAttributeText(CGLPixelFormatAttribute a);
void VuoCglPixelFormat_logDiff(CGLPixelFormatObj a, CGLPixelFormatObj b);

char *VuoCglRenderer_getText(int rendererID);

#ifdef __cplusplus
}
#endif
