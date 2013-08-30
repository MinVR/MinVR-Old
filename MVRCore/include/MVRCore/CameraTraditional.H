//========================================================================
// MinVR
// Platform:    Any
// API version: 1.0
//------------------------------------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2013 Regents of the University of Minnesota
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//========================================================================

#ifndef CAMERATRADITIONAL_H
#define CAMERATRADITIONAL_H

#include <glm/glm.hpp>
#include "MVRCore/AbstractCamera.H"

namespace MinVR {

class CameraTraditional : public AbstractCamera
{
public:
	CameraTraditional() { alwaysAssertM(false, "Traditional Camera is not yet implemented"); }
	virtual ~CameraTraditional() {}

	/*! @brief Updates the camera's current head position.
	*
	*  This method is called by the MVREngine to update the current head position
	*  for cameras which take into account the head position when calculating the projection matrix.
	*
	*  @param[in] The new head position.
	*/
	virtual void updateHeadTrackingFrame(glm::mat4 newHeadFrame) = 0;

	/*! @brief Sets the openGL matrices.
	*
	*  This method sets the openGL projection and modelview matrices based on the current
	*  head position and objectToWorldMatrix. The projection matrix is set so that the camera
	*  is centered between the eyes.
	*/
	virtual void applyProjectionAndCameraMatrices() = 0;

	/*! @brief Sets the openGL matrices.
	*
	*  This method sets the openGL projection and modelview matrices based on the current
	*  head position and objectToWorldMatrix. The projection matrix is set so that the camera
	*  is for the left eye for stereoscopic rendering.
	*/
	virtual void applyProjectionAndCameraMatricesForLeftEye() = 0;

	/*! @brief Sets the openGL matrices.
	*
	*  This method sets the openGL projection and modelview matrices based on the current
	*  head position and objectToWorldMatrix. The projection matrix is set so that the camera
	*  is for the right eye for stereoscopic rendering.
	*/
	virtual void applyProjectionAndCameraMatricesForRightEye() = 0;

	/*! @brief Sets the object to world matrix.
	*
	*  This method sets the transformation between the current object (model) space and world (room)
	*  coordinate system. It is used when setting the opengl modelview matrix.
	*/
	virtual void setObjectToWorldMatrix(glm::mat4 obj2World) = 0;
};

} // end namespace
#endif
