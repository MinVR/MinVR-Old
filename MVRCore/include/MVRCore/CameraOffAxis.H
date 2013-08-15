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

#include "G3DLite/G3DLite.h"
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
	CameraOffAxis(G3DLite::Vector3 topLeft, G3DLite::Vector3 topRight, G3DLite::Vector3 botLeft, G3DLite::Vector3 botRight,
				  G3DLite::CoordinateFrame initialHeadFrame, double interOcularDistance, double nearClipDist, double farClipDist);

	virtual ~CameraOffAxis();

	/*! @brief Updates the camera's current head position.
	*
	*  This method is called by the MVREngine to update the current head position.
	*  It also recalculates the current offaxis projection matrix based on the head position and modelview matrix.
	*
	*  @param[in] The new head position.
	*/
	virtual void updateHeadTrackingFrame(G3DLite::CoordinateFrame newHeadFrame);

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
	virtual void setObjectToWorldMatrix(G3DLite::CoordinateFrame obj2World);

	/*! @brief Gets the current location of the left eye.
	*
	*  Based on the current head position and interocular distance, this returns the left eye position
	*  
	*  @return A coordinate frame representing the translation and rotation for the left eye position.
	*/
	G3DLite::CoordinateFrame getLeftEyeFrame();

	/*! @brief Gets the current location of the right eye.
	*
	*  Based on the current head position and interocular distance, this returns the right eye position
	*  
	*  @return A coordinate frame representing the translation and rotation for the right eye position.
	*/
	G3DLite::CoordinateFrame getRightEyeFrame();

protected:

	G3DLite::Vector3 _topLeft;
	G3DLite::Vector3 _topRight;
	G3DLite::Vector3 _botLeft;
	G3DLite::Vector3 _botRight;
	G3DLite::CoordinateFrame _headFrame;
	double _iod;
	double _nearClip;
	double _farClip;
	double _halfWidth;
	double _halfHeight;
	G3DLite::CoordinateFrame _room2tile;

	G3DLite::Matrix4 _projection;
	G3DLite::Matrix4 _projectionLeft;
	G3DLite::Matrix4 _projectionRight;
	G3DLite::Matrix4 _view;
	G3DLite::Matrix4 _viewLeft;
	G3DLite::Matrix4 _viewRight;
	G3DLite::CoordinateFrame _object2World;

	G3DLite::Matrix4 _currentViewMatrix;

	void applyProjectionAndCameraMatrices(const G3DLite::Matrix4& projectionMat, const G3DLite::Matrix4& viewMat);
	G3DLite::Matrix4 invertYMat();

};

} //end namespace
#endif
