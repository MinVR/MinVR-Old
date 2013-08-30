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

#ifndef ABSTRACTMVRENGINE_H
#define ABSTRACTMVRENGINE_H

#include "MVRCore/AbstractMVRApp.H"
#include "MVRCore/AbstractWindow.H"
#include "MVRCore/ConfigMap.H"
#include "MVRCore/ConfigVal.H"
#include "MVRCore/WindowSettings.H"
#include "MVRCore/AbstractCamera.H"
#include "MVRCore/CameraOffAxis.H"
#include "MVRCore/AbstractInputDevice.H"
#include "MVRCore/InputDeviceTUIOClient.H"
#include "MVRCore/InputDeviceSpaceNav.H"
#include "MVRCore/InputDeviceVRPNAnalog.H"
#include "MVRCore/InputDeviceVRPNButton.H"
#include "MVRCore/InputDeviceVRPNTracker.H"
#include "MVRCore/RenderThread.H"
#include "MVRCore/DataFileUtils.H"
#include "MVRCore/Event.H"
#include <glm/glm.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/make_shared.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/empty_deleter.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

#define BOOST_ASSERT_MSG_OSTREAM std::cout
#include <boost/assert.hpp>

namespace MinVR {

/*! @brief Base class for vr engines
 *
 *  The MVREngine handles creating the windows, threads, devices, and mainloop.
 */
class AbstractMVREngine {
public:

	AbstractMVREngine();
	virtual ~AbstractMVREngine();

	/*! @brief Initialize the engine.
	 * 
	 *  Reads config files and parameters based on 'standard' command line arguments then
	 *  initializes graphics windows etc. based on this info.
	 */
	virtual void init(int argc, char **argv);

	/*! @brief Initialize the engine.
	 * 
	 *  Use this version of init instead and pass it config settings in the configMap argument
	 *  if you want to handle your own command line arguments
	 *
	 *	@param[in] A map of configuration parameters.
	 *  @param[in] Log file.
	 */
	virtual void init(ConfigMapRef configMap);

	/*! @brief Start the main loop.
	 *
	 *  This version of run takes over the main loop of the program, call it after init when you are
	 *  ready to start processing user input and rendering graphics
	 *
	 *  @param[in] A VR application reference.
	 */
	virtual void runApp(AbstractMVRAppRef app);

	/*! @brief Draws a frame of the graphics.
	 *
	 *  Use this version instead and call it repeatedly from your own mainloop if your program design
	 *  requires you to manage your own mainloop
	 *
	 *  @param[in] A VR application reference.
	 */
	virtual void runOneFrameOfApp(AbstractMVRAppRef app);

	/*! @brief Initialize thread specific variables
	 *
	 * Called from each renderthread to initialize needed variables
	 *
	 * @param[in] A thread specific id
	 * @param[in] Window reference for the calling renderthread.
	 */
	virtual void initializeContextSpecificVars(int threadId, WindowRef window);

	/*! @brief Initialize Boost Log v2.0
	 *
	 *  Sets up the logging file to the console and log file using Boost.Log v2.0
	 */
	virtual void initializeLogging();

protected:

	/*! @brief Creates windows and viewports
	 *
	 *  Called from init to create windows and viewports based on the vrsetup file
	 *  
	 *  @sa init
	 */
	virtual void setupWindowsAndViewports();

	/*! @brief Creates a window
	 *
	 *  Called from setupWindowsAndViewports to create a window
	 *
	 *  @param[in] Window settings specifying the dimensions, position, and properties of the window
	 *  @param[in] Array of cameras corresponding with viewports for the new window.
	 *
	 *  @sa setupWindowsAndViewports
	 */
	virtual WindowRef createWindow(WindowSettingsRef settings, std::vector<AbstractCameraRef> cameras) = 0;

	/*! @brief Creates Input Devices
	 *
	 *  Called from init to create input devices based on the vrsetup file
	 *  
	 *  @sa init
	 */
	virtual void setupInputDevices();

	/*! @brief Creates render threads.
	 *
	 *  Creates a new thread for each window specified in the vrsetup file. The threads are used
	 *  for multi-threaded rendering.
	 */
	virtual void setupRenderThreads();

	/*! @brief Poll the input devices for input.
	 *
	 *  Iterates through the input devices and windows polling each for input.
	 *  the input is sorted based on increasing time.
	 */
	virtual void pollUserInput();

	/*! @brief Updates head positions.
	 *
	 *  Updates each camera in every window with the new head location.
	 */
	virtual void updateProjectionForHeadTracking();

	AbstractMVRAppRef         _app;
	ConfigMapRef      _configMap;
	std::vector<EventRef> _events;
	std::vector<WindowRef>  _windows;
	std::vector<AbstractInputDeviceRef> _inputDevices;
	std::vector<RenderThreadRef> _renderThreads;
	boost::mutex _startRenderingMutex;
	boost::condition_variable _startRenderingCond;
	boost::mutex _renderingCompleteMutex;
	boost::condition_variable _renderingCompleteCond;
	boost::shared_ptr<boost::barrier> _swapBarrier;
	boost::posix_time::ptime _syncTimeStart;
	unsigned long _frameCount;
};

} // end namespace

#endif


