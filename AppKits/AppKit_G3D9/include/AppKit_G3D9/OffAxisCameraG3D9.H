//========================================================================
// MinVR - AppKit G3D9
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
