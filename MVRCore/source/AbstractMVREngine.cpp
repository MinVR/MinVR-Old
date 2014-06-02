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

#include "MVRCore/AbstractMVREngine.H"

#define BOOST_ASSERT_MSG_OSTREAM std::cout
#include <boost/assert.hpp>

namespace MinVR {

AbstractMVREngine::AbstractMVREngine()
{
}

AbstractMVREngine::~AbstractMVREngine()
{
}

BOOST_LOG_ATTRIBUTE_KEYWORD(tag_attr, "Tag", std::string)

void AbstractMVREngine::initializeLogging()
{
    boost::shared_ptr<boost::log::core> core = boost::log::core::get();

    // Create a backend and attach a couple of streams to it
    boost::shared_ptr<boost::log::sinks::text_ostream_backend> backend = boost::make_shared<boost::log::sinks::text_ostream_backend>();
    backend->add_stream(boost::shared_ptr< std::ostream >(&std::cout, boost::empty_deleter()));
    backend->add_stream(boost::shared_ptr< std::ostream >(new std::ofstream("log.txt")));

    // Enable auto-flushing after each log record written
    backend->auto_flush(true);

    // Wrap it into the frontend and register in the core.
    // The backend requires synchronization in the frontend.
    typedef boost::log::sinks::synchronous_sink<boost::log::sinks::text_ostream_backend> sink_t;
    boost::shared_ptr< sink_t > sink(new sink_t(backend));
    core->add_sink(sink);
	boost::log::add_common_attributes();
	sink->set_filter
    (
	boost::log::trivial::severity >= boost::log::trivial::info || (boost::log::expressions::has_attr(tag_attr) && tag_attr == "MinVR Core")
    );
}

void AbstractMVREngine::init(int argc, char **argv)
{
	initializeLogging();
	_configMap.reset(new ConfigMap(argc, argv, false));
	ConfigValMap::map = _configMap;
	
	_syncTimeStart = boost::posix_time::microsec_clock::local_time();
	setupWindowsAndViewports();
	setupInputDevices();
}

void AbstractMVREngine::init(ConfigMapRef configMap)
{
	_configMap = configMap;
	ConfigValMap::map = _configMap;

	_syncTimeStart = boost::posix_time::microsec_clock::local_time();
	setupWindowsAndViewports();
	setupInputDevices();
}

void AbstractMVREngine::setupWindowsAndViewports()
{
	glm::dmat4 initialHeadFrame = _configMap->get("InitialHeadFrame", glm::dmat4(1.0));
	
	// InterOcularDistance defaults to 2.5 inches (0.2083 ft). This assumes your coordinate system is in feet.
	double interOcularDistance = _configMap->get("InterOcularDistance", 0.2083);

	// Each window will have its own OpenGL graphics context and run in its own thread.
	// Many applications will only have 1 window, but for fastest performance in a CAVE
	// driven by a multiple GPU computer, you would want to create a separate Window
	// and OpenGL context for each wall, and render each of these in a separate thread.
	int nWindows = _configMap->get("NumWindows", 1);
	for (int w=0;w<nWindows;w++) {
		std::string winStr = "Window" + intToString(w+1) + "_";

		WindowSettingsRef wSettings(new WindowSettings());
		wSettings->width        = _configMap->get(winStr + "Width", wSettings->width);
		wSettings->height       = _configMap->get(winStr + "Height", wSettings->height);
		wSettings->xPos         = _configMap->get(winStr + "X", wSettings->xPos);
		wSettings->yPos         = _configMap->get(winStr + "Y", wSettings->yPos);
		wSettings->fullScreen   = _configMap->get(winStr + "FullScreen", wSettings->fullScreen);
		wSettings->resizable    = _configMap->get(winStr + "Resizable", wSettings->resizable);
		wSettings->framed       = _configMap->get(winStr + "Framed", wSettings->framed);
		wSettings->windowTitle  = _configMap->get(winStr + "Caption", wSettings->windowTitle);
		wSettings->useDebugContext = _configMap->get(winStr + "UseDebugContext", wSettings->useDebugContext);
		wSettings->msaaSamples  = _configMap->get(winStr + "MSAASamples", wSettings->msaaSamples);
		wSettings->rgbBits      = _configMap->get(winStr + "RGBBits", wSettings->rgbBits);
		wSettings->depthBits    = _configMap->get(winStr + "DepthBits", wSettings->depthBits);
		wSettings->stencilBits  = _configMap->get(winStr + "StencilBits", wSettings->stencilBits);
		wSettings->alphaBits    = _configMap->get(winStr + "AlphaBits", wSettings->alphaBits);
		wSettings->visible      = _configMap->get(winStr + "Visible", wSettings->visible);
		wSettings->useGPUAffinity = _configMap->get(winStr + "UseGPUAffinity", wSettings->useGPUAffinity);
		wSettings->stereo		= _configMap->get(winStr + "Stereo", wSettings->stereo);

		//wSettings.mouseVisible = _configMap->get(winStr + "MouseVisible", wSettings.mouseVisible);

		std::string stereoStr = _configMap->get(winStr + "StereoType", "Mono");
		if (stereoStr == "Mono") {
			wSettings->stereoType = WindowSettings::STEREOTYPE_MONO;
		}
		else if (stereoStr == "QuadBuffered") {
			wSettings->stereoType = WindowSettings::STEREOTYPE_QUADBUFFERED;
		}
		else if (stereoStr == "Checkerboard") {
			wSettings->stereoType = WindowSettings::STEREOTYPE_CHECKERBOARD;
		}
		else if (stereoStr == "InterlacedColumns") {
			wSettings->stereoType = WindowSettings::STEREOTYPE_INTERLACEDCOLUMNS;
		}
		else if (stereoStr == "InterlacedRows") {
			wSettings->stereoType = WindowSettings::STEREOTYPE_INTERLACEDROWS;
		}
		else if (stereoStr == "SideBySide") {
			wSettings->stereoType = WindowSettings::STEREOTYPE_SIDEBYSIDE;
		}
		else {
			std::stringstream ss;
			ss << "Fatal error: Unrecognized value for " + winStr + "StereoType: " + stereoStr;
			BOOST_ASSERT_MSG(false, ss.str().c_str());
		}


		// Within each window, you can have multiple viewports.  Each viewport will render
		// with a separate projection matrix, so this is used for drawing left and right eyes
		// in a side-by-side layout or for rendering different walls of a CAVE in the situation
		// where you stretch a large window across a virtual desktop that covers all the walls
		// of the cave and render using one thread rather than using a separate window and 
		// rendering thread for each wall.
		int nViewports = _configMap->get(winStr + "NumViewports", 1);
		std::vector<AbstractCameraRef> cameras;
		for (int v=0;v<nViewports;v++) {
			std::string viewportStr = winStr + "Viewport" + intToString(v+1) + "_";
			
			int width    = _configMap->get(viewportStr + "Width", wSettings->width);
			int height   = _configMap->get(viewportStr + "Height", wSettings->height);
			int x        = _configMap->get(viewportStr + "X", 0);
			int y        = _configMap->get(viewportStr + "Y", 0);
			wSettings->viewports.push_back(MinVR::Rect2D::xywh(x,y,width,height));

			std::string cameraStr = _configMap->get(viewportStr + "CameraType", "OffAxis");
			if (cameraStr == "OffAxis") {
				glm::dvec3 topLeft  = _configMap->get(viewportStr + "TopLeft", glm::dvec3(-1.0, 1.0, 0.0));
				glm::dvec3 topRight = _configMap->get(viewportStr + "TopRight", glm::dvec3(1.0, 1.0, 0.0));
				glm::dvec3 botLeft  = _configMap->get(viewportStr + "BotLeft", glm::dvec3(-1.0, -1.0, 0.0));
				glm::dvec3 botRight = _configMap->get(viewportStr + "BotRight", glm::dvec3(1.0, -1.0, 0.0));
				double nearClip = _configMap->get(viewportStr + "NearClip", 0.01);
				double farClip  = _configMap->get(viewportStr + "FarClip", 1000.0);
				AbstractCameraRef cam(new CameraOffAxis(topLeft, topRight, botLeft, botRight, initialHeadFrame, interOcularDistance, nearClip, farClip));
				cameras.push_back(cam);
			}
			else if (cameraStr == "Traditional") {
				// TODO: Implement this for use with systems like desktop haptics.
				BOOST_ASSERT_MSG(false, "Traditional camera not yet implemented");
			}
			else {
				std::stringstream ss;
				ss << "Fatal error: Unrecognized value for " << viewportStr << "CameraType: " << cameraStr;
				BOOST_ASSERT_MSG(false, ss.str().c_str());
			}
		}

		WindowRef window = createWindow(wSettings, cameras);
		_windows.push_back(window);
	}

	for (int i=0;i<_windows.size();i++) {
		_windows[i]->updateHeadTrackingForAllViewports(initialHeadFrame);
	}
}

void AbstractMVREngine::setupInputDevices()
{
	std::string devicesFile = _configMap->get("InputDevicesFile", "");
	if (devicesFile != "") {
		ConfigMapRef devicesMap(new ConfigMap(DataFileUtils::findDataFile(devicesFile)));
		std::string inputDevices = devicesMap->get( "InputDevices", "");
		std::vector<std::string> devnames = splitStringIntoArray(inputDevices);

		for (int i=0;i<devnames.size();i++) {
			std::string type = devicesMap->get(devnames[i] + "_Type", "");

			if (type == "InputDeviceVRPNAnalog") {
				_inputDevices.push_back(AbstractInputDeviceRef(new InputDeviceVRPNAnalog(devnames[i], devicesMap)));
			}
			else if (type == "InputDeviceVRPNButton") {
				_inputDevices.push_back(AbstractInputDeviceRef(new InputDeviceVRPNButton(devnames[i], devicesMap)));
			}
			else if (type == "InputDeviceVRPNTracker") {
				_inputDevices.push_back(AbstractInputDeviceRef(new InputDeviceVRPNTracker(devnames[i], devicesMap)));
			}
			else if (type == "InputDeviceTUIOClient") { 
				_inputDevices.push_back(AbstractInputDeviceRef(new InputDeviceTUIOClient(devnames[i], devicesMap)));
			}
			else if (type == "InputDeviceSpaceNav") {
				_inputDevices.push_back(AbstractInputDeviceRef(new InputDeviceSpaceNav(devnames[i], devicesMap)));
			}
			else {
				std::stringstream ss;
				ss << "Fatal error: Unrecognized input device type" << type;
				BOOST_ASSERT_MSG(false, ss.str().c_str());
			}
		}
	}
}

void AbstractMVREngine::initializeContextSpecificVars(int threadId, WindowRef window)
{
}

void AbstractMVREngine::setupRenderThreads()
{
	_renderThreads.clear();

	RenderThread::numRenderingThreads = _windows.size();
	RenderThread::renderingState = RenderThread::RENDERING_WAIT;
	RenderThread::numThreadsReceivedRenderingComplete = 0;
	RenderThread::numThreadsReceivedStartRendering = 0;
	RenderThread::nextThreadId = 0;
	RenderThread::numThreadsInitComplete = 0;

	_swapBarrier = boost::shared_ptr<boost::barrier>(new boost::barrier(RenderThread::numRenderingThreads));

	for(int i=0; i < _windows.size(); i++) {
		RenderThreadRef thread(new RenderThread(_windows[i], this, _app, _swapBarrier.get(), &_threadsInitializedMutex, &_threadsInitializedCond, &_startRenderingMutex, &_renderingCompleteMutex, &_startRenderingCond, &_renderingCompleteCond));
		_renderThreads.push_back(thread);
	}
}

void AbstractMVREngine::runApp(AbstractMVRAppRef app)
{
	_app = app;

	setupRenderThreads();
	// Wait for threads to finish being initialized
	boost::unique_lock<boost::mutex> threadsInitializedLock(_threadsInitializedMutex);
	while (RenderThread::numThreadsInitComplete < _windows.size()) {
		_threadsInitializedCond.wait(threadsInitializedLock);
	}
	threadsInitializedLock.unlock();

	_app->postInitialization();

	_frameCount = 0;
	
	bool quit = false;
	while (!quit) {
		runOneFrameOfApp(app);
	}
}

void AbstractMVREngine::runOneFrameOfApp(AbstractMVRAppRef app)
{
	// It is possible that this is called by itself rather than from runApp(), if so make sure _app is assigned
	// and that the renderthreads are created.
	if (_app != app) {
		_app = app;
		_frameCount = 0;
	}
	if (_renderThreads.size() == 0) {
		setupRenderThreads();
		// Wait for threads to finish being initialized
		boost::unique_lock<boost::mutex> threadsInitializedLock(_threadsInitializedMutex);
		while (RenderThread::numThreadsInitComplete < _windows.size()) {
			_threadsInitializedCond.wait(threadsInitializedLock);
		}
		threadsInitializedLock.unlock();
		_app->postInitialization();
	}

	pollUserInput();
	updateProjectionForHeadTracking();

	boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
	boost::posix_time::time_duration diff = now - _syncTimeStart;
	double syncTime = diff.total_seconds();
	_app->doUserInputAndPreDrawComputation(_events, syncTime);

	//std::cout << "Notifying rendering threads to start rendering frame: "<<_frameCount++<<std::endl;
	_startRenderingMutex.lock();
	RenderThread::renderingState = RenderThread::RENDERING_START;
	_startRenderingCond.notify_all();
	_startRenderingMutex.unlock();

	// Wait for threads to finish rendering
	boost::unique_lock<boost::mutex> renderingCompleteLock(_renderingCompleteMutex);
	while (RenderThread::numThreadsReceivedRenderingComplete < _windows.size()) {
		_renderingCompleteCond.wait(renderingCompleteLock);
	}
	//std::cout << "All threads finished rendering"<<std::endl;
	RenderThread::numThreadsReceivedRenderingComplete = 0;
	renderingCompleteLock.unlock();
}

void AbstractMVREngine::pollUserInput()
{
	_events.clear();
	for (int i=0;i<_windows.size();i++) {
		_windows[i]->pollForInput(_events);
	}
	for (int i=0;i<_inputDevices.size();i++) { 
		_inputDevices[i]->pollForInput(_events);
	}
	
	//TODO: ideally we want to sort the events by time stamp, but this seems to be flipping some tracker events
	// out of order. Currently we get better results not sorting. Still needs to be debugged
	//std::stable_sort(_events.begin(), _events.end());
}

void AbstractMVREngine::updateProjectionForHeadTracking() 
{
	// Use the most recent Head_Tracker event as the head position
	int i = (int)_events.size()-1;
	while ((i >= 0) && (_events[i]->getName() != "Head_Tracker")) {
		i--;
	}
	if (i >= 0) {
		for (int j=0;j<_windows.size();j++) {
			_windows[j]->updateHeadTrackingForAllViewports(_events[i]->getCoordinateFrameData());
		}
	}
} 

} // end namespace

