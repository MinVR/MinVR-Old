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

#include "MVRCore/AbstractMVREngine.H"

using namespace G3DLite;

namespace MinVR {

AbstractMVREngine::AbstractMVREngine()
{
}

AbstractMVREngine::~AbstractMVREngine()
{
}

void AbstractMVREngine::init(int argc, char **argv)
{
	_log = new G3DLite::Log("log.txt");

	ConfigMapRef myMap = new ConfigMap(argc, argv, _log, false);
	init(myMap);
}

void AbstractMVREngine::init(ConfigMapRef configMap)
{
	_configMap = configMap;

	_syncTimeStart = boost::posix_time::microsec_clock::local_time();
	setupWindowsAndViewports();
	setupInputDevices();
}

void AbstractMVREngine::setupWindowsAndViewports()
{
	G3DLite::CoordinateFrame initialHeadFrame = _configMap->get("InitialHeadFrame", CoordinateFrame());
	
	// InterOcularDistance defaults to 2.5 inches (0.2083 ft). This assumes your coordinate system is in feet.
	float interOcularDistance = _configMap->get("InterOcularDistance", 0.2083);

	// Each window will have its own OpenGL graphics context and run in its own thread.
	// Many applications will only have 1 window, but for fastest performance in a CAVE
	// driven by a multiple GPU computer, you would want to create a separate Window
	// and OpenGL context for each wall, and render each of these in a separate thread.
	int nWindows = _configMap->get("NumWindows", 1);
	for (int w=0;w<nWindows;w++) {
		std::string winStr = "Window" + intToString(w+1) + "_";

		WindowSettingsRef wSettings = new WindowSettings();
		wSettings->width        = _configMap->get(winStr + "Width", wSettings->width);
		wSettings->height       = _configMap->get(winStr + "Height", wSettings->height);
		wSettings->xPos         = _configMap->get(winStr + "X", wSettings->xPos);
		wSettings->yPos         = _configMap->get(winStr + "Y", wSettings->yPos);
		wSettings->fullScreen   = _configMap->get(winStr + "FullScreen", wSettings->fullScreen);
		wSettings->resizable    = _configMap->get(winStr + "Resizable", wSettings->resizable);
		wSettings->framed       = _configMap->get(winStr + "Framed", wSettings->framed);
		wSettings->windowTitle  = _configMap->get(winStr + "Caption", wSettings->windowTitle);
		wSettings->useDebugContext = _configMap->get(winStr + "UseDebugContext", wSettings->useDebugContext);
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
			alwaysAssertM(false, "Fatal error: Unrecognized value for " + winStr + "StereoType: " + stereoStr);
		}


		// Within each window, you can have multiple viewports.  Each viewport will render
		// with a separate projection matrix, so this is used for drawing left and right eyes
		// in a side-by-side layout or for rendering different walls of a CAVE in the situation
		// where you stretch a large window across a virtual desktop that covers all the walls
		// of the cave and render using one thread rather than using a separate window and 
		// rendering thread for each wall.
		int nViewports = _configMap->get(winStr + "NumViewports", 1);
		G3DLite::Array<AbstractCameraRef> cameras;
		for (int v=0;v<nViewports;v++) {
			std::string viewportStr = winStr + "Viewport" + intToString(v+1) + "_";
			
			int width    = _configMap->get(viewportStr + "Width", wSettings->width);
			int height   = _configMap->get(viewportStr + "Height", wSettings->height);
			int x        = _configMap->get(viewportStr + "X", 0.0);
			int y        = _configMap->get(viewportStr + "Y", 0.0);
			wSettings->viewports.append(G3DLite::Rect2D::xywh(x,y,width,height));

			std::string cameraStr = _configMap->get(viewportStr + "CameraType", "OffAxis");
			if (cameraStr == "OffAxis") {
				Vector3 topLeft  = _configMap->get(viewportStr + "TopLeft", Vector3(-1, 1, 0));
				Vector3 topRight = _configMap->get(viewportStr + "TopRight", Vector3(1, 1, 0));
				Vector3 botLeft  = _configMap->get(viewportStr + "BotLeft", Vector3(-1, -1, 0));
				Vector3 botRight = _configMap->get(viewportStr + "BotRight", Vector3(1, -1, 0));
				float nearClip = _configMap->get(viewportStr + "NearClip", 0.01);
				float farClip  = _configMap->get(viewportStr + "FarClip", 1000.0);
				AbstractCameraRef cam = new CameraOffAxis(topLeft, topRight, botLeft, botRight, initialHeadFrame, interOcularDistance, nearClip, farClip);
				cameras.append(cam);
			}
			else if (cameraStr == "Traditional") {
				// TODO: Implement this for use with systems like desktop haptics.
				alwaysAssertM(false, "Traditional camera not yet implemented");
			}
			else {
				alwaysAssertM(false, "Fatal error: Unrecognized value for " + viewportStr + "CameraType: " + cameraStr);
			}
		}

		WindowRef window = createWindow(wSettings, cameras);
		_windows.append(window);
	}

	for (int i=0;i<_windows.size();i++) {
		_windows[i]->updateHeadTrackingForAllViewports(initialHeadFrame);
	}
}

void AbstractMVREngine::setupInputDevices()
{
	Array<std::string> devnames = splitStringIntoArray(_configMap->get( "InputDevices", ""));

	for (int i=0;i<devnames.size();i++) {
		std::string type = _configMap->get(devnames[i] + "_Type", "");

		if (type == "InputDeviceVRPNAnalog") {
			_inputDevices.append(new InputDeviceVRPNAnalog(devnames[i], _log, _configMap));
		}
		else if (type == "InputDeviceVRPNButton") {
			_inputDevices.append(new InputDeviceVRPNButton(devnames[i], _log, _configMap));
		}
		else if (type == "InputDeviceVRPNTracker") {
			_inputDevices.append(new InputDeviceVRPNTracker(devnames[i], _log, _configMap));
		}
		else if (type == "InputDeviceTUIOClient") { 
			_inputDevices.append(new InputDeviceTUIOClient(devnames[i], _log, _configMap));
		}
		else if (type == "InputDeviceSpaceNav") {
			_inputDevices.append(new InputDeviceSpaceNav(devnames[i], _log, _configMap));
		}
		else {
			alwaysAssertM(false, "Fatal error: Unrecognized input device type" + type);
		}
	}
}

void AbstractMVREngine::setupRenderThreads()
{
	_renderThreads.clear();

	RenderThread::numRenderingThreads = _windows.size();
	RenderThread::renderingState = RenderThread::RENDERING_WAIT;
	RenderThread::numThreadsReceivedRenderingComplete = 0;
	RenderThread::numThreadsReceivedStartRendering = 0;
	RenderThread::nextThreadId = 0;

	_swapBarrier = boost::shared_ptr<boost::barrier>(new boost::barrier(RenderThread::numRenderingThreads));

	for(int i=0; i < _windows.size(); i++) {
		RenderThreadRef thread = new RenderThread(_windows[i], _app, _swapBarrier.get(), &_startRenderingMutex, &_renderingCompleteMutex, &_startRenderingCond, &_renderingCompleteCond);
		_renderThreads.append(thread);
	}
}

void AbstractMVREngine::runApp(AbstractMVRAppRef app)
{
	_app = app;

	setupRenderThreads();

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
	}

	pollUserInput();
	updateProjectionForHeadTracking();

	boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
	boost::posix_time::time_duration diff = now - _syncTimeStart;
	double syncTime = diff.total_seconds();
	_app->doUserInputAndPreDrawComputation(_events, syncTime);

	std::cout << "Notifying rendering threads to start rendering frame: "<<_frameCount++<<std::endl;
	_startRenderingMutex.lock();
	RenderThread::renderingState = RenderThread::RENDERING_START;
	_startRenderingCond.notify_all();
	_startRenderingMutex.unlock();

	// Wait for threads to finish rendering
	boost::unique_lock<boost::mutex> renderingCompleteLock(_renderingCompleteMutex);
	while (RenderThread::numThreadsReceivedRenderingComplete < _windows.size()) {
		_renderingCompleteCond.wait(renderingCompleteLock);
	}
	std::cout << "All threads finished rendering"<<std::endl;
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
	_events.sort();
}

void AbstractMVREngine::updateProjectionForHeadTracking() 
{
	// Use the most recent Head_Tracker event as the head position
	int i = _events.size()-1;
	while ((i >= 0) && (_events[i]->getName() != "Head_Tracker")) {
		i--;
	}
	if (i >= 0) {
		for (int i=0;i<_windows.size();i++) {
			_windows[i]->updateHeadTrackingForAllViewports(_events[i]->getCoordinateFrameData());
		}
	}
} 

} // end namespace

