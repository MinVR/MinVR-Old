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

#ifndef CAMERAOFFAXIS_H
#define CAMERAOFFAXIS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
#include "MVRCore/AbstractCamera.H"

namespace MinVR {

/*! @Brief An offaxis VR camera
 *
 *  This derived class represents an offaxis view frustrum.
 *  It should be used head tracking in front of a flat display tile or in a VR CAVE.
 */
class CameraOffAxis : public AbstractCamera
{
public:
	CameraOffAxis(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 botLeft, glm::vec3 botRight,
				  glm::mat4 initialHeadFrame, double interOcularDistance, double nearClipDist, double farClipDist);

	virtual ~CameraOffAxis();

	/*! @brief Updates the camera's current head position.
	*
	*  This method is called by the MVREngine to update the current head position.
	*  It also recalculates the current offaxis projection matrix based on the head position and modelview matrix.
	*
	*  @param[in] The new head position.
	*/
	virtual void updateHeadTrackingFrame(glm::mat4 newHeadFrame);

	/*! @brief Sets the openGL matrices.
	*
	*  This method sets the openGL projection and modelview matrices as if the camera is centered
	*  between the eyes. This is used for mono rendering.
	*/
	virtual void applyProjectionAndCameraMatrices();

	/*! @brief Sets the openGL matrices.
	*
	*  This method sets the openGL projection and modelview matrices as if the camera
	*  is located at the left eye. This is used for stereoscopic rendering.
	*/
	virtual void applyProjectionAndCameraMatricesForLeftEye();

	/*! @brief Sets the openGL matrices.
	*
	*  This method sets the openGL projection and modelview matrices as if the camera
	*  is located at the right eye. This is used for stereoscopic rendering.
	*/
	virtual void applyProjectionAndCameraMatricesForRightEye();

	/*! @brief Sets the object to world matrix.
	*
	*  This method sets the transformation between the current object (model) space and world (room)
	*  coordinate system. It is used when setting the opengl modelview matrix.
	*/
	virtual void setObjectToWorldMatrix(glm::mat4 obj2World);

	/*! @brief Gets the current location of the left eye.
	*
	*  Based on the current head position and interocular distance, this returns the left eye position
	*  
	*  @return A coordinate frame representing the translation and rotation for the left eye position.
	*/
	glm::mat4 getLeftEyeFrame();

	/*! @brief Gets the current location of the right eye.
	*
	*  Based on the current head position and interocular distance, this returns the right eye position
	*  
	*  @return A coordinate frame representing the translation and rotation for the right eye position.
	*/
	glm::mat4 getRightEyeFrame();

protected:

	glm::vec3 _topLeft;
	glm::vec3 _topRight;
	glm::vec3 _botLeft;
	glm::vec3 _botRight;
	glm::mat4 _headFrame;
	double _iod;
	double _nearClip;
	double _farClip;
	double _halfWidth;
	double _halfHeight;
	glm::mat4 _room2tile;

	glm::mat4 _projection;
	glm::mat4 _projectionLeft;
	glm::mat4 _projectionRight;
	glm::mat4 _view;
	glm::mat4 _viewLeft;
	glm::mat4 _viewRight;
	glm::mat4 _object2World;

	glm::mat4 _currentViewMatrix;

	void applyProjectionAndCameraMatrices(const glm::mat4& projectionMat, const glm::mat4& viewMat);
	glm::mat4 invertYMat();
	glm::mat4 perspectiveProjection(double left, double right, double bottom, double top, double nearval, double farval, float upDirection = -1.0);

};

} //end namespace
#endif
