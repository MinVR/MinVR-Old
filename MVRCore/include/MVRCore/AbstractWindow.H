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
	size_t getNumViewports() { return _viewports.size(); }
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
