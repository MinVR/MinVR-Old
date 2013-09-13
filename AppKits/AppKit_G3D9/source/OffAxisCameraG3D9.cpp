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

#include "AppKit_G3D9/OffAxisCameraG3D9.H"

namespace MinVR {

OffAxisCameraG3D9::OffAxisCameraG3D9(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 botLeft, glm::vec3 botRight,
				  glm::mat4 initialHeadFrame, double interOcularDistance, double nearClipDist, double farClipDist) : CameraOffAxis(topLeft, topRight, botLeft, botRight, initialHeadFrame, interOcularDistance, nearClipDist, farClipDist)
{
	_renderDevice = nullptr;
}

OffAxisCameraG3D9::~OffAxisCameraG3D9()
{
}

void OffAxisCameraG3D9::applyProjectionAndCameraMatrices(const glm::mat4& projectionMat, const glm::mat4& viewMat)
{
	debugAssertM(_renderDevice != nullptr, "OffAxisCameraG3D9::setRenderDevice has not been called yet");

	// convert from glm to G3D Matrix4 (i.e. column major to row major, and without the invertY since g3d does that later)
	glm::mat4 invProj = invertYMat() * projectionMat;
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

	_renderDevice->setProjectionAndCameraMatrix(G3D::Projection(g3dProj), g3dView);
}

void OffAxisCameraG3D9::setObjectToWorldMatrix(glm::mat4 obj2World)
{
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