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

/**
 * \author Bret Jackson
 *
 * \file  RenderThread.cpp
 * \brief Individual thread tied to a window and context for rendering
 *
 */

#include "MVRCore/RenderThread.H"
#include "MVRCore/AbstractMVREngine.H"

using namespace std;

namespace MinVR {

RenderThread::RenderingState RenderThread::renderingState = RenderThread::RENDERING_WAIT;
int RenderThread::numThreadsReceivedStartRendering = 0;
int RenderThread::numThreadsReceivedRenderingComplete = 0;
int RenderThread::numRenderingThreads = 0;
int RenderThread::nextThreadId = 0;
int RenderThread::numThreadsInitComplete = 0;

RenderThread::RenderThread(WindowRef window, AbstractMVREngine* engine, AbstractMVRAppRef app, boost::barrier* swapBarrier,  boost::mutex* initializedMutex, boost::condition_variable* initializedCondition,
	boost::mutex* startRenderingMutex, boost::mutex* renderingCompleteMutex, boost::condition_variable* startRenderingCond, boost::condition_variable* renderingCompleteCond)
{
	_window = window;
	_engine = engine;
	_app = app;
	_swapBarrier = swapBarrier;
	_initMutex = initializedMutex;
	_initCond = initializedCondition;
	_startRenderingMutex = startRenderingMutex;
	_renderingCompleteMutex = renderingCompleteMutex;
	_startRenderingCond = startRenderingCond;
	_renderingCompleteCond = renderingCompleteCond;
	_threadId = RenderThread::nextThreadId;
	RenderThread::nextThreadId++;

	_thread = boost::shared_ptr<boost::thread>(new boost::thread(&RenderThread::render, this));
}

RenderThread::~RenderThread()
{
	if (_thread) {
		_thread->join();
	}
}

void RenderThread::render()
{
	_window->makeContextCurrent();
	
	initExtensions();
	initStereoFramebufferAndTextures();
	initStereoCompositeShader();
	setShaderVariables();

	GLenum err;
	if((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "openGL ERROR before init context specific: "<<err<<std::endl;
	}

	_engine->initializeContextSpecificVars(_threadId, _window);
	_app->initializeContextSpecificVars(_threadId, _window);

	if((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "openGL ERROR in start of render(): "<<err<<std::endl;
	}

	// Signal that the thread is initialized
	_initMutex->lock();
	numThreadsInitComplete++;
	_initCond->notify_all();
	_initMutex->unlock();

	bool running = true;
	while (running) {

		// Wait for the main thread to signal that it's ok to start rendering
		boost::unique_lock<boost::mutex> startRenderingLock(*_startRenderingMutex);
		while (renderingState == RENDERING_WAIT) {
			_startRenderingCond->wait(startRenderingLock);
		}
		if (renderingState == RENDERING_TERMINATE) {
			// RENDERING_TERMINATE is a special flag used to quit the application and cleanup all the threads nicely
			startRenderingLock.unlock();
			return;
		}
		numThreadsReceivedStartRendering++;
		// if this is the last thread to receive the StartRendering signal, then reset the flags for use in the next frame
		if (numThreadsReceivedStartRendering >= numRenderingThreads) {
			renderingState = RENDERING_WAIT;
			numThreadsReceivedStartRendering = 0;
		}
		startRenderingLock.unlock();

		//cout <<"\t Thread "<<_threadId<<" received start rendering"<<endl;

		// Draw the scene
		// Monoscopic
		if (_window->getSettings()->stereoType == WindowSettings::STEREOTYPE_MONO || _window->getSettings()->stereo == false) {
			glDrawBuffer(GL_BACK);
			glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			for (int v=0; v < _window->getNumViewports(); v++) {
				MinVR::Rect2D viewport = _window->getViewport(v);
				glViewport(viewport.x0(), viewport.y0(), viewport.width(), viewport.height());
				_window->getCamera(v)->applyProjectionAndCameraMatrices();
				_app->drawGraphics(_threadId, _window->getCamera(v), _window);
			}  
		}
		
		// Quad Buffered Stereo
		else if (_window->getSettings()->stereoType == WindowSettings::STEREOTYPE_QUADBUFFERED) {
			// Left Eye
			glDrawBuffer(GL_BACK_LEFT);
			glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			for (int v=0; v < _window->getNumViewports(); v++) {
				MinVR::Rect2D viewport = _window->getViewport(v);
				glViewport(viewport.x0(), viewport.y0(), viewport.width(), viewport.height());
				_window->getCamera(v)->applyProjectionAndCameraMatricesForLeftEye();
				_app->drawGraphics(_threadId, _window->getCamera(v), _window);
			}
			// Right Eye
			glDrawBuffer(GL_BACK_RIGHT);
			glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			for (int v=0; v < _window->getNumViewports(); v++) {
				MinVR::Rect2D viewport = _window->getViewport(v);
				glViewport(viewport.x0(), viewport.y0(), viewport.width(), viewport.height());
				_window->getCamera(v)->applyProjectionAndCameraMatricesForRightEye();
				_app->drawGraphics(_threadId, _window->getCamera(v), _window);
			} 
		}

		// Side by Side Stereo Images, Left Eye on the left half of the screen and Right Eye on the right
		else if (_window->getSettings()->stereoType == WindowSettings::STEREOTYPE_SIDEBYSIDE) {
			glDrawBuffer(GL_BACK);
			glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			// Left Eye
			for (int v=0; v < _window->getNumViewports(); v++) {
				MinVR::Rect2D viewport = _window->getViewport(v);
				glViewport(viewport.x0(), viewport.y0(), viewport.width()/2, viewport.height());
				_window->getCamera(v)->applyProjectionAndCameraMatricesForLeftEye();
				_app->drawGraphics(_threadId, _window->getCamera(v), _window);
			}
			// Right Eye
			for (int v=0; v < _window->getNumViewports(); v++) {
				MinVR::Rect2D viewport = _window->getViewport(v);
				glViewport(viewport.x0()+viewport.width()/2, viewport.y0(), viewport.width()/2, viewport.height());
				_window->getCamera(v)->applyProjectionAndCameraMatricesForRightEye();
				_app->drawGraphics(_threadId, _window->getCamera(v), _window);
			} 
		}

		// Draw using either checkerboard or interlaced stereo
		else {
			// bind a framebuffer object
			glBindFramebuffer(GL_FRAMEBUFFER, _stereoFBO);
		
			//Set lefteye texture
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _leftEyeTexture, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			for (int v=0; v < _window->getNumViewports(); v++) {
				MinVR::Rect2D viewport = _window->getViewport(v);
				glViewport(viewport.x0(), viewport.y0(), viewport.width(), viewport.height());
				_window->getCamera(v)->applyProjectionAndCameraMatricesForLeftEye();
				_app->drawGraphics(_threadId, _window->getCamera(v), _window);
			}

			//Set righteye texture
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _rightEyeTexture, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			for (int v=0; v < _window->getNumViewports(); v++) {
				MinVR::Rect2D viewport = _window->getViewport(v);
				glViewport(viewport.x0(), viewport.y0(), viewport.width(), viewport.height());
				_window->getCamera(v)->applyProjectionAndCameraMatricesForRightEye();
				_app->drawGraphics(_threadId, _window->getCamera(v), _window);
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glUseProgram(_stereoProgram);
			glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, _leftEyeTexture);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, _rightEyeTexture);

			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2, GL_FLOAT, 0, 0);
			glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
			glDisableClientState(GL_VERTEX_ARRAY);

			glBindBuffer(GL_ARRAY_BUFFER_ARB, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, 0);
			glUseProgram(0);
		}

		//cout << "\tThread "<<_threadId<<" finished rendering"<<endl;

		// Wait for the other threads to get here before swapping buffers
		_swapBarrier->wait();

		//cout << "\tThread "<<_threadId<<" swapping buffers"<<endl;
		_window->swapBuffers();

		// Signal that this rendering thread has completed drawing
		_renderingCompleteMutex->lock();
		numThreadsReceivedRenderingComplete++;
		_renderingCompleteCond->notify_all();
		_renderingCompleteMutex->unlock();
	}
}

void RenderThread::initExtensions()
{
#ifdef _WIN32
	// get pointers to GL functions
	pglGenFramebuffers                     = (PFNGLGENFRAMEBUFFERSPROC)wglGetProcAddress("glGenFramebuffers");
	pglDeleteFramebuffers                  = (PFNGLDELETEFRAMEBUFFERSPROC)wglGetProcAddress("glDeleteFramebuffers");
	pglBindFramebuffer                     = (PFNGLBINDFRAMEBUFFERPROC)wglGetProcAddress("glBindFramebuffer");
	pglCheckFramebufferStatus              = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)wglGetProcAddress("glCheckFramebufferStatus");
	pglGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)wglGetProcAddress("glGetFramebufferAttachmentParameteriv");
	pglGenerateMipmap                      = (PFNGLGENERATEMIPMAPPROC)wglGetProcAddress("glGenerateMipmap");
	pglFramebufferTexture2D                = (PFNGLFRAMEBUFFERTEXTURE2DPROC)wglGetProcAddress("glFramebufferTexture2D");
	pglFramebufferRenderbuffer             = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)wglGetProcAddress("glFramebufferRenderbuffer");
	pglGenRenderbuffers                    = (PFNGLGENRENDERBUFFERSPROC)wglGetProcAddress("glGenRenderbuffers");
	pglDeleteRenderbuffers                 = (PFNGLDELETERENDERBUFFERSPROC)wglGetProcAddress("glDeleteRenderbuffers");
	pglBindRenderbuffer                    = (PFNGLBINDRENDERBUFFERPROC)wglGetProcAddress("glBindRenderbuffer");
	pglRenderbufferStorage                 = (PFNGLRENDERBUFFERSTORAGEPROC)wglGetProcAddress("glRenderbufferStorage");
	pglGetRenderbufferParameteriv          = (PFNGLGETRENDERBUFFERPARAMETERIVPROC)wglGetProcAddress("glGetRenderbufferParameteriv");
	pglIsRenderbuffer                      = (PFNGLISRENDERBUFFERPROC)wglGetProcAddress("glIsRenderbuffer");

	// check once again FBO extension
	if(!pglGenFramebuffers || !pglDeleteFramebuffers || !pglBindFramebuffer || !pglCheckFramebufferStatus ||
		!pglGetFramebufferAttachmentParameteriv || !pglGenerateMipmap || !pglFramebufferTexture2D || !pglFramebufferRenderbuffer ||
		!pglGenRenderbuffers || !pglDeleteRenderbuffers || !pglBindRenderbuffer || !pglRenderbufferStorage ||
		!pglGetRenderbufferParameteriv || !pglIsRenderbuffer)
	{
		BOOST_ASSERT_MSG(false, "Video card does NOT support GL_ARB_framebuffer_object.");
	}
	
	pglCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
	pglCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
	pglShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
	pglCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
	pglGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC)wglGetProcAddress("glGetObjectParameterivARB");
	pglAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
	pglLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
	pglGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
	pglGetProgramivARB = (PFNGLGETPROGRAMIVARBPROC)wglGetProcAddress("glGetProgramivARB");
	pglUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
	pglGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
	pglUniform2f = (PFNGLUNIFORM2FPROC)wglGetProcAddress("glUniform2f");
	pglUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");

	if (!pglCreateProgram || !pglCreateShader || !pglShaderSource || !pglCompileShader || !pglGetObjectParameterivARB ||
		!pglAttachShader || !pglLinkProgram || !pglGetShaderiv || !pglGetProgramivARB || !pglUseProgram ||
		!pglGetUniformLocation || !pglUniform2f || !pglUniform1i )
	{
		BOOST_ASSERT_MSG(false, "Video card does NOT support loading shader extensions.");
	}

	pglBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
	pglGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
	pglBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");

	if (!pglBindBuffer || !pglGenBuffers || !pglBufferData) {
		BOOST_ASSERT_MSG(false, "Video card does NOT support vertex buffer objects.");
	}

	pglActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");

	if (!pglActiveTexture) {
		BOOST_ASSERT_MSG(false, "Video card does NOT support glActiveTexture");
	}

#endif
}

void RenderThread::initStereoCompositeShader()
{
	// Only bother if we actually need the shader
	if (_window->getSettings()->stereoType == WindowSettings::STEREOTYPE_CHECKERBOARD ||
		_window->getSettings()->stereoType == WindowSettings::STEREOTYPE_INTERLACEDCOLUMNS ||
		_window->getSettings()->stereoType == WindowSettings::STEREOTYPE_INTERLACEDROWS) {

		
		GLuint vertexShader, fragmentShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);	
	
		std::string vertexShaderName = DataFileUtils::findDataFile("shaders/stereo.vert");
		BOOST_ASSERT_MSG(boost::filesystem::exists(vertexShaderName), "Unable to load vertex shader for stereo in RenderThread.cpp. File not found");
		const char* vs = readWholeFile(vertexShaderName).c_str();

		std::string fragShaderName = "";
		if (_window->getSettings()->stereoType == WindowSettings::STEREOTYPE_CHECKERBOARD) {
			fragShaderName = DataFileUtils::findDataFile("shaders/stereo-checkerboard.frag");
		}
		else if (_window->getSettings()->stereoType == WindowSettings::STEREOTYPE_INTERLACEDCOLUMNS) {
			fragShaderName = DataFileUtils::findDataFile("shaders/stereo-interlacedcolumns.frag");
		}
		else if (_window->getSettings()->stereoType == WindowSettings::STEREOTYPE_INTERLACEDROWS) {
			fragShaderName = DataFileUtils::findDataFile("shaders/stereo-interlacedrows.frag");
		}

		BOOST_ASSERT_MSG(boost::filesystem::exists(fragShaderName), "Unable to load fragment shader for stereo in RenderThread.cpp. File not found");
		const char* fs = readWholeFile(fragShaderName).c_str();
	
		glShaderSource(vertexShader, 1, &vs,NULL);
		glShaderSource(fragmentShader, 1, &fs,NULL);
	
		glCompileShader(vertexShader);
		glCompileShader(fragmentShader);
	
		_stereoProgram = glCreateProgram();
		
		glAttachShader(_stereoProgram,vertexShader);
		glAttachShader(_stereoProgram,fragmentShader);
	
		glLinkProgram(_stereoProgram);
	}
}

void RenderThread::initStereoFramebufferAndTextures() {
	// Only bother if we actually need the textures and fbo for stereo
	if (_window->getSettings()->stereoType == WindowSettings::STEREOTYPE_CHECKERBOARD ||
		_window->getSettings()->stereoType == WindowSettings::STEREOTYPE_INTERLACEDCOLUMNS ||
		_window->getSettings()->stereoType == WindowSettings::STEREOTYPE_INTERLACEDROWS) {

		// Create eye textures
		glGenTextures(1, &_leftEyeTexture);
		glGenTextures(1, &_rightEyeTexture);

		glBindTexture(GL_TEXTURE_2D, _leftEyeTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _window->getWidth(), _window->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); 

		glBindTexture(GL_TEXTURE_2D, _rightEyeTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _window->getWidth(), _window->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); 

		glBindTexture(GL_TEXTURE_2D, 0);
	
		// Setup framebuffer object
		glGenFramebuffers(1, &_stereoFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, _stereoFBO);

		// create a renderbuffer object to store depth info
		glGenRenderbuffers(1, &_depthRBO);
		glBindRenderbuffer(GL_RENDERBUFFER, _depthRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _window->getWidth(), _window->getHeight());
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		// attach a texture to FBO color attachement point
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _leftEyeTexture, 0);

		// attach a renderbuffer to depth attachment point
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthRBO);

		// check FBO status
		GLenum e = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		std::string message = "";
		switch (e) {
			case GL_FRAMEBUFFER_UNDEFINED:
				message = "FBO Undefined";
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT :
				message = "FBO Incomplete Attachment";
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT :
				message = "FBO Missing Attachment";
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER :
				message = "FBO Incomplete Draw Buffer";
				break;
			case GL_FRAMEBUFFER_UNSUPPORTED :
				message = "FBO Unsupported";
				break;
			case GL_FRAMEBUFFER_COMPLETE:
				message = "FBO OK";
				break;
			default:
				message = "FBO Problem?";
		}

		if (e != GL_FRAMEBUFFER_COMPLETE) {
			BOOST_ASSERT_MSG(false, message.c_str());
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//Setup fullscreen quad vbo
		_fullscreenVertices[0] = -1.0;
		_fullscreenVertices[1] = -1.0;
		_fullscreenVertices[2] = 1.0;
		_fullscreenVertices[3] = -1.0;
		_fullscreenVertices[4] = 1.0;
		_fullscreenVertices[5] = 1.0;
		_fullscreenVertices[6] = -1.0;
		_fullscreenVertices[7] =  1.0;
		_fullscreenIndices[0] = 0;
		_fullscreenIndices[1] = 1;
		_fullscreenIndices[2] = 2;
		_fullscreenIndices[3] = 3;

		//Create VBO
		glGenBuffers( 1, &_vertexBuffer );
		glBindBuffer( GL_ARRAY_BUFFER, _vertexBuffer );
		glBufferData( GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), _fullscreenVertices, GL_STATIC_DRAW );

		//Create IBO
		glGenBuffers( 1, &_indexBuffer );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, _indexBuffer );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), _fullscreenIndices, GL_STATIC_DRAW );
	}
}

void RenderThread::setShaderVariables()
{
	// Only bother if we actually need the textures and fbo for stereo
	if (_window->getSettings()->stereoType == WindowSettings::STEREOTYPE_CHECKERBOARD ||
		_window->getSettings()->stereoType == WindowSettings::STEREOTYPE_INTERLACEDCOLUMNS ||
		_window->getSettings()->stereoType == WindowSettings::STEREOTYPE_INTERLACEDROWS)
	{
		glUseProgram(_stereoProgram);

		GLint screenSizeLoc = glGetUniformLocation(_stereoProgram, "screenSize");
		glUniform2f(screenSizeLoc, _window->getWidth(), _window->getHeight());

		GLuint leftEyeTexLoc  = glGetUniformLocation(_stereoProgram, "leftEyeTexture");
		GLuint rightEyeTexLoc = glGetUniformLocation(_stereoProgram, "rightEyeTexture");
		glUniform1i(leftEyeTexLoc, 0);
		glUniform1i(rightEyeTexLoc, 1);

		glUseProgram(0);
	}
}

} // end namespace