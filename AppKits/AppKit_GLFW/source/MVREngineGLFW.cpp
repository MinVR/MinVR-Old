//========================================================================
// MinVR - AppKit GLFW
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