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

#include "MVRCore/CameraOffAxis.H"
#include "G3DLite/G3DLite.h"
#include <gl/GL.h>
#include <gl/GLU.h>

using namespace G3DLite;

namespace MinVR
{

CameraOffAxis::CameraOffAxis(Vector3 topLeft, Vector3 topRight, Vector3 botLeft, Vector3 botRight,
	CoordinateFrame initialHeadFrame, double interOcularDistance, 
	double nearClipDist, double farClipDist) : AbstractCamera()
{
	_topLeft = topLeft;
	_topRight = topRight;
	_botLeft = botLeft;
	_botRight = botRight;
	_headFrame = initialHeadFrame;
	_iod = interOcularDistance;
	_nearClip = nearClipDist;
	_farClip = farClipDist;
	_halfWidth = (_topRight - _topLeft).length() / 2.0;
	_halfHeight = (_topRight - _botRight).length() / 2.0;

	Vector3 center = (topLeft + topRight + botLeft + botRight) / 4.0;
	Vector3 x = (topRight - topLeft).unit();
	Vector3 y = (topLeft - botLeft).unit();
	Vector3 z = x.cross(y).unit();
	Matrix3 rot(x.x, y.x, z.x, x.y, y.y, z.y, x.z, y.z, z.z);
	CoordinateFrame tile2room(rot, center);
	_room2tile = tile2room.inverse();
}

CameraOffAxis::~CameraOffAxis()
{
}

void CameraOffAxis::updateHeadTrackingFrame(CoordinateFrame newHeadFrame)
{
	_headFrame = newHeadFrame;

	// 1. Get the center of the camera (the eye) position from the head position
	CoordinateFrame head2Room = _headFrame;
	CoordinateFrame leftEye2Room = getLeftEyeFrame();
	CoordinateFrame rightEye2Room = getRightEyeFrame();
  
	// 2. Setup projection matrix
	Vector3 head = (_room2tile * head2Room).translation;
	Vector3 left = (_room2tile * leftEye2Room).translation;
	Vector3 right = (_room2tile * rightEye2Room).translation;
	

	double lHead = (-_halfWidth - head.x);
	double rHead = (_halfWidth - head.x);
	double lLeft = (-_halfWidth - left.x);
	double rLeft = (_halfWidth - left.x);
	double lRight = (-_halfWidth - right.x);
	double rRight = (_halfWidth - right.x);

	// y and z for head and eyes are the same so don't need to calculate three times
	double b = (-_halfHeight - head.y);
	double t = (_halfHeight - head.y);
	double dist = head.z;
	double k = _nearClip / dist;

	// 3. Add eye position to the projection (eye is in tile coordinates)
	CoordinateFrame r2t = CoordinateFrame(-head) * _room2tile;
	CoordinateFrame r2tLeft = CoordinateFrame(-left) * _room2tile;
	CoordinateFrame r2tRight = CoordinateFrame(-right) * _room2tile;

	_projection = invertYMat() * Matrix4::perspectiveProjection(lHead*k, rHead*k, b*k, t*k, _nearClip, _farClip);
	_projectionLeft = invertYMat() * Matrix4::perspectiveProjection(lLeft*k, rLeft*k, b*k, t*k, _nearClip, _farClip);
	_projectionRight = invertYMat() * Matrix4::perspectiveProjection(lRight*k, rRight*k, b*k, t*k, _nearClip, _farClip);
	_view = Matrix4(r2t);//.inverse();
	_viewLeft = Matrix4(r2tLeft);//.inverse();
	_viewRight = Matrix4(r2tRight);//.inverse();
}

Matrix4 CameraOffAxis::invertYMat()
{
	static Matrix4 M(1,  0, 0, 0,
					  0, -1, 0, 0,
					  0,  0, 1, 0,
					  0,  0, 0, 1);
	return M;
}

CoordinateFrame	CameraOffAxis::getLeftEyeFrame()
{
	return _headFrame * CoordinateFrame(Vector3(-_iod/2.0, 0.0, 0.0));
}

CoordinateFrame	CameraOffAxis::getRightEyeFrame()
{
	return _headFrame * CoordinateFrame(Vector3( _iod/2.0, 0.0, 0.0));
}

void CameraOffAxis::applyProjectionAndCameraMatrices()
{
	applyProjectionAndCameraMatrices(_projection, _view);
}

void CameraOffAxis::applyProjectionAndCameraMatricesForLeftEye()
{
	applyProjectionAndCameraMatrices(_projectionLeft, _viewLeft);
}

void CameraOffAxis::applyProjectionAndCameraMatricesForRightEye()
{
	applyProjectionAndCameraMatrices(_projectionRight, _viewRight);
}

void CameraOffAxis::setObjectToWorldMatrix(G3DLite::CoordinateFrame obj2World)
{
	_object2World = obj2World;
	glMatrixMode(GL_MODELVIEW);
	Matrix4 modelView = _currentViewMatrix * _object2World;
	GLfloat matrix[16];
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            // Transpose
            matrix[c * 4 + r] = modelView[r][c];
        }
    }
	glLoadMatrixf(matrix);
}


void CameraOffAxis::applyProjectionAndCameraMatrices(const Matrix4& projectionMat, const Matrix4& viewMat)
{
	_currentViewMatrix = viewMat;
	glMatrixMode(GL_PROJECTION);
    GLfloat matrix[16];
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            // Transpose
            matrix[c * 4 + r] = projectionMat[r][c];
        }
    }
	glLoadMatrixf(matrix);

	glMatrixMode(GL_MODELVIEW);
	Matrix4 modelView = viewMat * _object2World;
	for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            // Transpose
            matrix[c * 4 + r] = modelView[r][c];
        }
    }
	glLoadMatrixf(matrix);
}

} // End namespace