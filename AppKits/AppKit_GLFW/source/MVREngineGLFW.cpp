//========================================================================
// MinVR - AppKit GLFW
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

#include "AppKit_GLFW/MVREngineGLFW.H"

namespace MinVR {

MVREngineGLFW::MVREngineGLFW() : AbstractMVREngine()
{
	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) {
		BOOST_ASSERT_MSG(false, "Cannot initialize glfw");
	}
}

MVREngineGLFW::~MVREngineGLFW()
{
	glfwTerminate();
}

void MVREngineGLFW::runApp(AbstractMVRAppRef app)
{
	_app = app;

	setupRenderThreads();
	
	bool quit = false;
	while (!quit) {
		runOneFrameOfApp(app);

		for (int i=0; i<_windows.size(); i++) {
			if (glfwWindowShouldClose(((WindowGLFW*)_windows[i].get())->getWindowPtr())) {
				quit = true;
			}
		}
	}

	// Signal threads to terminate and cleanup
	_startRenderingMutex.lock();
	RenderThread::renderingState = RenderThread::RENDERING_TERMINATE;
	_startRenderingCond.notify_all();
	_startRenderingMutex.unlock();

	_renderThreads.clear();
}

WindowRef MVREngineGLFW::createWindow(WindowSettingsRef settings, std::vector<AbstractCameraRef> cameras)
{
	WindowRef window(new WindowGLFW(settings, cameras));
	return window;
}

void MVREngineGLFW::error_callback(int error, const char* description)
{
	BOOST_ASSERT_MSG(false, description);
}

} // end namespace