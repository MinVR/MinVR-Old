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

#ifndef OFFAXISCAMERAG3D9_H
#define OFFAXISCAMERAG3D9_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
#include "MVRCore/CameraOffAxis.H"
#include "MVRCore/StringUtils.H"
#include <G3D/Vector3.h>
#include <G3D/Matrix3.h>
#include <G3D/Matrix4.h>
#include <G3D/CoordinateFrame.h>
#include <G3D/Projection.h>
#include <G3D/debugAssert.h>
#include <GLG3D/RenderDevice.h>

namespace MinVR {

/*! @Brief An offaxis VR camera
 *
 *  This derived class represents an offaxis view frustrum.
 *  It should be used head tracking in front of a flat display tile or in a VR CAVE.
 */
class OffAxisCameraG3D9 : public CameraOffAxis
{
public:
	OffAxisCameraG3D9(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 botLeft, glm::vec3 botRight,
				  glm::mat4 initialHeadFrame, double interOcularDistance, double nearClipDist, double farClipDist);

	virtual ~OffAxisCameraG3D9();

	/*! @brief Sets the object to world matrix.
	*
	*  This method sets the transformation between the current object (model) space and world (room)
	*  coordinate system. It is used when setting the opengl modelview matrix.
	*/
	virtual void setObjectToWorldMatrix(glm::mat4 obj2World);

	/*! @brief Set the associated render device
	*/
	void setRenderDevice(G3D::RenderDevice* rd);

protected:
	G3D::RenderDevice* _renderDevice;

	virtual void applyProjectionAndCameraMatrices(const glm::mat4& projectionMat, const glm::mat4& viewMat);
};

} //end namespace
#endif
