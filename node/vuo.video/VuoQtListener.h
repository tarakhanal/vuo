/**
 * @file
 * VuoQtListener implementation.
 *
 * @copyright Copyright © 2012–2018 Kosada Incorporated.
 * This code may be modified and distributed under the terms of the MIT License.
 * For more information, see http://vuo.org/license.
 */

#ifndef NS_RETURNS_INNER_POINTER
#define NS_RETURNS_INNER_POINTER
#endif
#import <QTKit/QTkit.h>

#include "VuoVideoFrame.h"
#include "VuoVideoInputDevice.h"

/**
 * Manages receiving video frames via a QTCaptureSession.
 */
@interface VuoQtListener : NSObject
{
	QTMovie								*mMovie;							///< ?
	QTCaptureSession					*mCaptureSession;					///< Manages a list of frame receivers.
	QTCaptureDeviceInput				*mCaptureDeviceInput;				///< The device from which to receive frames.
	QTCaptureDecompressedVideoOutput	*mCaptureDecompressedVideoOutput;	///< A frame receiver.

	CVImageBufferRef 					mCurrentImageBuffer;				///< The latest received frame.
	void (*callback)(VuoVideoFrame);												///< Called when a frame is received.
	double								lastFrameReceived;					///< The timestamp at which the last frame was received.

	NSArray 							*mVideoInputDevices;				///< All available video input devices. @todo Make this a singleton?

	VuoVideoInputDevice 				desiredDevice;						///< The device the user has requested.
	BOOL 								userWantsRunning;					///< Was startRunning called (without calling stopRunning)?
	CVOpenGLTextureCacheRef 			textureCache;						///< GL_TEXTURE_RECTANGLEs from the video feed
	CIContext *ciContext; ///< Fallback for when CVOpenGLTextureCacheRef fails.
}

/**
 *	Initialize a QTListener with the device name or id.  The listener will attempt to find the device
 *	first using ID, then name.  If a matching device is not found, the listener will wait for a device
 *	matching the description until it is either registered, or told to stop.
 */
- (BOOL) initWithDevice:(VuoVideoInputDevice)device callback:(void(*)(VuoVideoFrame))receivedFrame;
- (BOOL) isInitialized;
- (BOOL) isRunning;

- (void) setInputDevice:(VuoVideoInputDevice)device;

- (void) startRunning;
- (void) stopRunning;

- (void) devicesDidChange:(NSNotification *)notification;
- (void) refreshDevices;
- (void) setCaptureDevice:(QTCaptureDevice*)device;

- (void) setCallback:(void(*)(VuoVideoFrame))receivedFrame;
- (void) setWidth:(VuoInteger)width height:(VuoInteger)height;
+ (char*) formatTypeString:(int)key;

@end
