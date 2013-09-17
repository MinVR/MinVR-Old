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

#ifndef WINDOW_H
#define WINDOW_H

#include <boost/shared_ptr.hpp>
#include <glm/glm.hpp>
#include "MVRCore/AbstractCamera.H"
#include "MVRCore/Event.H"
#include "MVRCore/WindowSettings.H"
#include "MVRCore/Rect2D.H"
#include <vector>

namespace MinVR {

typedef std::shared_ptr<class AbstractWindow> WindowRef;

/*! @brief Base class for windows
*/
class AbstractWindow
{
public:
	
	AbstractWindow(WindowSettingsRef settings, std::vector<AbstractCameraRef> cameras);
	virtual ~AbstractWindow();

	/*! @brief Collect window events
	 *
	 *  This method should be implemented for each derived class.
	 *  It collects window events and adds them to the events array.
	 */
	virtual void pollForInput(std::vector<EventRef> &events) = 0;
	
	/*! @brief Swaps window buffers
	 *
	 *  This method should be implemented for each derived class.
	 *  It swaps the front and back opengl buffers.
	 */
	virtual void swapBuffers() = 0;

	/*! @brief Make the context current.
	 *
	 *  This method should be implemented for each derived class.
	 *  It makes the associated context current on the calling thread.
	 *
	 *  @note A context can only be current on one thread at a time. RenderThread calls this method for each thread.
	 */
	virtual void makeContextCurrent() = 0;

	/*! @brief Release the context current.
	 *
	 *  This method should be implemented for each derived class.
	 *  It releases the associated context that is current on the calling thread.
	 *
	 *  @note A context can only be current on one thread at a time.
	 */
	virtual void releaseContext() = 0;

	/*! @brief Updates the current head position.
	 *
	 *  This method updates the head position for each camera that is associated with a specific viewport
	 *
	 *  @param[in] The new head position.
	 */
	virtual void updateHeadTrackingForAllViewports(glm::mat4 headFrame);

	WindowSettings::StereoType getStereoType() { return _settings->stereoType; }
	int getNumViewports() { return _viewports.size(); }
	MinVR::Rect2D getViewport(int n) { return _viewports[n]; }
	AbstractCameraRef getCamera(int n) { return _cameras[n]; }
	WindowSettingsRef getSettings() { return _settings; }

	virtual int getWidth() = 0;
	virtual int getHeight() = 0;
	virtual int getXPos() = 0;
	virtual int getYPos() = 0;

protected:

	WindowSettingsRef _settings;
	std::vector<MinVR::Rect2D>    _viewports;
	std::vector<AbstractCameraRef> _cameras;
};


} // end namespace

#endif
