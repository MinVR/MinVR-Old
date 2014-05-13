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

#include "AppKit_G3D9/OffAxisCameraG3D9.H"

namespace MinVR {

OffAxisCameraG3D9::OffAxisCameraG3D9(glm::dvec3 topLeft, glm::dvec3 topRight, glm::dvec3 botLeft, glm::dvec3 botRight,
				  glm::dmat4 initialHeadFrame, double interOcularDistance, double nearClipDist, double farClipDist) : CameraOffAxis(topLeft, topRight, botLeft, botRight, initialHeadFrame, interOcularDistance, nearClipDist, farClipDist)
{
	_renderDevice = nullptr;
}

OffAxisCameraG3D9::~OffAxisCameraG3D9()
{
}

void OffAxisCameraG3D9::applyProjectionAndCameraMatrices(const glm::dmat4& projectionMat, const glm::dmat4& viewMat)
{
	debugAssertM(_renderDevice != nullptr, "OffAxisCameraG3D9::setRenderDevice has not been called yet");

	_currentViewMatrix = viewMat;
	_currentProjMatrix = projectionMat;

	// convert from glm to G3D Matrix4 (i.e. column major to row major, and without the invertY since g3d does that later)
	glm::dmat4 invProj = invertYMat() * projectionMat;
	G3D::Matrix4 g3dProj(invProj[0][0], invProj[1][0], invProj[2][0], invProj[3][0],
						 invProj[0][1], invProj[1][1], invProj[2][1], invProj[3][1],
						 invProj[0][2], invProj[1][2], invProj[2][2], invProj[3][2],
						 invProj[0][3], invProj[1][3], invProj[2][3], invProj[3][3]);

	G3D::Matrix3 rotation(viewMat[0][0], viewMat[1][0], viewMat[2][0],
						  viewMat[0][1], viewMat[1][1], viewMat[2][1],
						  viewMat[0][2], viewMat[1][2], viewMat[2][2]);
	G3D::Vector3 trans(viewMat[3][0], viewMat[3][1], viewMat[3][2]);
	G3D::CoordinateFrame g3dView(rotation, trans);
	g3dView = g3dView.inverse();

	_renderDevice->setProjectionMatrix(g3dProj);
	_renderDevice->setCameraToWorldMatrix(g3dView);
}

void OffAxisCameraG3D9::setObjectToWorldMatrix(glm::mat4 obj2World)
{
	_obj2World = obj2World
	debugAssertM(_renderDevice != nullptr, "OffAxisCameraG3D9::setRenderDevice has not been called yet");

	G3D::Matrix4 g3dMat(obj2World[0][0], obj2World[1][0], obj2World[2][0], obj2World[3][0],
						obj2World[0][1], obj2World[1][1], obj2World[2][1], obj2World[3][1],
						obj2World[0][2], obj2World[1][2], obj2World[2][2], obj2World[3][2],
						obj2World[0][3], obj2World[1][3], obj2World[2][3], obj2World[3][3]);
	G3D::CoordinateFrame frame = g3dMat.approxCoordinateFrame();
	_renderDevice->setObjectToWorldMatrix(frame);
}

void OffAxisCameraG3D9::setRenderDevice(G3D::RenderDevice* rd)
{
	_renderDevice = rd;
}

} // namespace