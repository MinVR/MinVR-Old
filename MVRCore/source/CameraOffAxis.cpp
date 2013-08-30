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
#ifdef WIN32
#define NOMINMAX
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

namespace MinVR
{

CameraOffAxis::CameraOffAxis(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 botLeft, glm::vec3 botRight,
	glm::mat4 initialHeadFrame, double interOcularDistance, 
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
	_halfWidth = glm::length(_topRight - _topLeft) / 2.0;
	_halfHeight = glm::length(_topRight - _botRight) / 2.0;

	glm::vec3 center = (topLeft + topRight + botLeft + botRight);
	center.x = center.x / 4.0;
	center.y = center.y / 4.0;
	center.z = center.z / 4.0;
	glm::vec3 x = glm::normalize(topRight - topLeft);
	glm::vec3 y = glm::normalize(topLeft - botLeft);
	glm::vec3 z = glm::normalize(glm::cross(x, y));
	glm::mat4 tile2room(x.x, y.x, z.x, center.x,
						x.y, y.y, z.y, center.y,
						x.z, y.z, z.z, center.z,
						0, 0, 0, 1); 
	_room2tile = glm::inverse(tile2room);
}

CameraOffAxis::~CameraOffAxis()
{
}

void CameraOffAxis::updateHeadTrackingFrame(glm::mat4 newHeadFrame)
{
	_headFrame = newHeadFrame;

	// 1. Get the center of the camera (the eye) position from the head position
	glm::mat4 head2Room = _headFrame;
	glm::mat4 leftEye2Room = getLeftEyeFrame();
	glm::mat4 rightEye2Room = getRightEyeFrame();
  
	// 2. Setup projection matrix
	glm::vec3 head = glm::column((_room2tile * head2Room), 3).xyz;
	glm::vec3 left = glm::column((_room2tile * leftEye2Room), 3).xyz;
	glm::vec3 right = glm::column((_room2tile * rightEye2Room), 3).xyz;
	

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
	glm::mat4 r2t = glm::column(glm::mat4(1.0), 3, glm::vec4(-head, 1.0)) * _room2tile;
	glm::mat4 r2tLeft = glm::column(glm::mat4(1.0), 3, glm::vec4(-left, 1.0)) * _room2tile;
	glm::mat4 r2tRight = glm::column(glm::mat4(1.0), 3, glm::vec4(-right, 1.0)) * _room2tile;

	_projection = invertYMat() * perspectiveProjection(lHead*k, rHead*k, b*k, t*k, _nearClip, _farClip);
	_projectionLeft = invertYMat() * perspectiveProjection(lLeft*k, rLeft*k, b*k, t*k, _nearClip, _farClip);
	_projectionRight = invertYMat() * perspectiveProjection(lRight*k, rRight*k, b*k, t*k, _nearClip, _farClip);
	_view = r2t;//.inverse();
	_viewLeft = r2tLeft;//.inverse();
	_viewRight = r2tRight;//.inverse();
}

glm::mat4 CameraOffAxis::invertYMat()
{
	static glm::mat4 M(1,  0, 0, 0,
					  0, -1, 0, 0,
					  0,  0, 1, 0,
					  0,  0, 0, 1);
	return M;
}

glm::mat4 CameraOffAxis::perspectiveProjection(double left, double right, double bottom, double top, double nearval, double farval, float upDirection)
{
    double x, y, a, b, c, d;

    x = (2.0*nearval) / (right-left);
    y = (2.0*nearval) / (top-bottom);
    a = (right+left) / (right-left);
    b = (top+bottom) / (top-bottom);

    if (farval >= std::numeric_limits<double>::max()) {
       // Infinite view frustum
       c = -1.0;
       d = -2.0 * nearval;
    } else {
       c = -(farval+nearval) / (farval-nearval);
       d = -(2.0*farval*nearval) / (farval-nearval);
    }

    BOOST_ASSERT_MSG(abs(upDirection) == 1.0, "upDirection must be -1 or +1");
    y *= upDirection;
    b *= upDirection;

    return glm::mat4(
        (float)x,  0,  (float)a,  0,
        0,  (float)y,  (float)b,  0,
        0,  0,  (float)c,  (float)d,
        0,  0, -1,  0);
}

glm::mat4 CameraOffAxis::getLeftEyeFrame()
{
	return _headFrame * glm::column(glm::mat4(1.0), 3, glm::vec4(-_iod/2.0, 0.0, 0.0, 1.0));
}

glm::mat4 CameraOffAxis::getRightEyeFrame()
{
	return _headFrame * glm::column(glm::mat4(1.0), 3, glm::vec4(_iod/2.0, 0.0, 0.0, 1.0));
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

void CameraOffAxis::setObjectToWorldMatrix(glm::mat4 obj2World)
{
	_object2World = obj2World;
	glMatrixMode(GL_MODELVIEW);
	glm::mat4 modelView = _currentViewMatrix * _object2World;
	GLfloat matrix[16];
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            // Transpose
            matrix[c * 4 + r] = modelView[r][c];
        }
    }
	glLoadMatrixf(matrix);
}


void CameraOffAxis::applyProjectionAndCameraMatrices(const glm::mat4& projectionMat, const glm::mat4& viewMat)
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
	glm::mat4 modelView = viewMat * _object2World;
	for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            // Transpose
            matrix[c * 4 + r] = modelView[r][c];
        }
    }
	glLoadMatrixf(matrix);
}

} // End namespace