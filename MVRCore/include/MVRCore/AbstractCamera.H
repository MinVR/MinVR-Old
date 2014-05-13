//========================================================================
// MinVR
// Platform:    Any
// API version: 1.0
//------------------------------------------------------------------------
// Copyright (c) 2013 Regents of the University of Minnesota
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice, this
//   list of conditions and the following disclaimer in the documentation and/or
//   other materials provided with the distribution.
//
// * Neither the name of the University of Minnesota, nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//========================================================================

#ifndef ABSTRACTCAMERA_H
#define ABSTRACTCAMERA_H

#include <boost/shared_ptr.hpp>
#include <glm/glm.hpp>

namespace MinVR {

typedef std::shared_ptr<class AbstractCamera> AbstractCameraRef;

class AbstractCamera
{
public:
	AbstractCamera() {}
	virtual ~AbstractCamera() {}

	/*! @brief Updates the camera's current head position.
	*
	*  This method is called by the MVREngine to update the current head position
	*  for cameras which take into account the head position when calculating the projection matrix.
	*
	*  @param[in] The new head position.
	*
	*  @remarks This should be implemented by any derived classes.
	*/
	virtual void updateHeadTrackingFrame(glm::dmat4 newHeadFrame) = 0;

	/*! @brief Sets the openGL matrices.
	*
	*  This method sets the openGL projection and modelview matrices based on the current
	*  head position and objectToWorldMatrix. The projection matrix is set so that the camera
	*  is centered between the eyes.
	*
	*  @remarks This should be implemented by any derived classes.
	*/
	virtual void applyProjectionAndCameraMatrices() = 0;

	/*! @brief Sets the openGL matrices.
	*
	*  This method sets the openGL projection and modelview matrices based on the current
	*  head position and objectToWorldMatrix. The projection matrix is set so that the camera
	*  is for the left eye for stereoscopic rendering.
	*
	*  @remarks This should be implemented by any derived classes.
	*/
	virtual void applyProjectionAndCameraMatricesForLeftEye() = 0;

	/*! @brief Sets the openGL matrices.
	*
	*  This method sets the openGL projection and modelview matrices based on the current
	*  head position and objectToWorldMatrix. The projection matrix is set so that the camera
	*  is for the right eye for stereoscopic rendering.
	*
	*  @remarks This should be implemented by any derived classes.
	*/
	virtual void applyProjectionAndCameraMatricesForRightEye() = 0;

	/*! @brief Sets the object to world matrix.
	*
	*  This method sets the transformation between the current object (model) space and world (room)
	*  coordinate system. It is used when setting the opengl modelview matrix.
	*
	*  @remarks This should be implemented by any derived classes.
	*/
	virtual void setObjectToWorldMatrix(glm::dmat4 obj2World) = 0;
};

} // end namespace
#endif
