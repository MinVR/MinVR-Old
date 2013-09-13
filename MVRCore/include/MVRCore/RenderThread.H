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

#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include "MVRCore/AbstractMVRApp.H"
#include "MVRCore/AbstractWindow.H"
#include "MVRCore/DataFileUtils.H"
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <boost/filesystem.hpp>
#include "MVRCore/StringUtils.H"

#ifdef WIN32
#define NOMINMAX
#include <windows.h>
#endif
#include <GL/gl.h>
#include "GL/glext.h"

namespace MinVR {

// forward declaration
class AbstractMVREngine;

typedef boost::shared_ptr<class RenderThread> RenderThreadRef;

class RenderThread
{
public:
	enum RenderingState {
		RENDERING_WAIT = 0,
		RENDERING_START,
		RENDERING_TERMINATE
	};

	RenderThread(WindowRef window, AbstractMVREngine* engine, AbstractMVRAppRef app, boost::barrier* swapBarrier, boost::mutex* initializedMutex, boost::condition_variable* initializedCondition, boost::mutex* startRenderingMutex, boost::mutex* renderingCompleteMutex, boost::condition_variable* startRenderingCond, boost::condition_variable* renderingCompleteCond);
	~RenderThread();

	static RenderingState renderingState;
	static int numThreadsReceivedStartRendering;
	static int numThreadsReceivedRenderingComplete;
	static int numRenderingThreads;
	static int nextThreadId;
	static int numThreadsInitComplete;

private:
	void render();
	void initExtensions();
	void initStereoCompositeShader();
	void initStereoFramebufferAndTextures();
	void setShaderVariables();
	
	WindowRef _window;
	AbstractMVREngine* _engine;
	AbstractMVRAppRef _app;
	bool	_rendering;
	int		_numRenderingThreads;
	boost::shared_ptr<boost::thread> _thread;
	boost::barrier* _swapBarrier;
	boost::mutex* _initMutex;
	boost::mutex* _startRenderingMutex;
	boost::mutex* _renderingCompleteMutex;
	boost::condition_variable* _initCond;
	boost::condition_variable* _startRenderingCond;
	boost::condition_variable* _renderingCompleteCond;
	int _threadId;
	
	GLuint _stereoFBO;
	GLuint _leftEyeTexture;
	GLuint _rightEyeTexture;
	GLuint _depthRBO;
	GLhandleARB _stereoProgram;
	GLfloat _fullscreenVertices[8];
	GLuint _fullscreenIndices[4];
	GLuint _vertexBuffer;
	GLuint _indexBuffer;

	// Unfortunately windows does not default to supporting opengl > 1.1
	// This is a hack to load the framebuffer and shader extensions needed to support
	// interlaced and checkerboard stereo rendering. We have chosen not to use glew to avoid
	// an additional dependency that can cause versioning conflicts with app kits that also use glew.
#ifdef _WIN32
	// Framebuffer object
	PFNGLGENFRAMEBUFFERSPROC                     pglGenFramebuffers;                      // FBO name generation procedure
	PFNGLDELETEFRAMEBUFFERSPROC                  pglDeleteFramebuffers;                   // FBO deletion procedure
	PFNGLBINDFRAMEBUFFERPROC                     pglBindFramebuffer;                      // FBO bind procedure
	PFNGLCHECKFRAMEBUFFERSTATUSPROC              pglCheckFramebufferStatus;               // FBO completeness test procedure
	PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC pglGetFramebufferAttachmentParameteriv;  // return various FBO parameters
	PFNGLGENERATEMIPMAPPROC                      pglGenerateMipmap;                       // FBO automatic mipmap generation procedure
	PFNGLFRAMEBUFFERTEXTURE2DPROC                pglFramebufferTexture2D;                 // FBO texdture attachement procedure
	PFNGLFRAMEBUFFERRENDERBUFFERPROC             pglFramebufferRenderbuffer;              // FBO renderbuffer attachement procedure
	// Renderbuffer object
	PFNGLGENRENDERBUFFERSPROC                    pglGenRenderbuffers;                     // renderbuffer generation procedure
	PFNGLDELETERENDERBUFFERSPROC                 pglDeleteRenderbuffers;                  // renderbuffer deletion procedure
	PFNGLBINDRENDERBUFFERPROC                    pglBindRenderbuffer;                     // renderbuffer bind procedure
	PFNGLRENDERBUFFERSTORAGEPROC                 pglRenderbufferStorage;                  // renderbuffer memory allocation procedure
	PFNGLGETRENDERBUFFERPARAMETERIVPROC          pglGetRenderbufferParameteriv;           // return various renderbuffer parameters
	PFNGLISRENDERBUFFERPROC                      pglIsRenderbuffer;                       // determine renderbuffer object type
	// Shaders
	PFNGLCREATEPROGRAMPROC						 pglCreateProgram;
	PFNGLCREATESHADERPROC						 pglCreateShader;
	PFNGLSHADERSOURCEPROC						 pglShaderSource;
	PFNGLCOMPILESHADERPROC					     pglCompileShader;
	PFNGLGETOBJECTPARAMETERIVARBPROC			 pglGetObjectParameterivARB;
	PFNGLATTACHSHADERPROC						 pglAttachShader;
	PFNGLLINKPROGRAMPROC						 pglLinkProgram;
	PFNGLGETSHADERIVPROC						 pglGetShaderiv;
	PFNGLGETPROGRAMIVARBPROC					 pglGetProgramivARB;
	PFNGLUSEPROGRAMPROC							 pglUseProgram;
	PFNGLGETUNIFORMLOCATIONPROC					 pglGetUniformLocation;
	PFNGLUNIFORM2FPROC							 pglUniform2f;
	PFNGLUNIFORM1IPROC							 pglUniform1i;
	// VBO
	PFNGLBINDBUFFERPROC							 pglBindBuffer;
	PFNGLGENBUFFERSPROC							 pglGenBuffers;
	PFNGLBUFFERDATAPROC							 pglBufferData;
	// Textures
	PFNGLACTIVETEXTUREPROC						 pglActiveTexture;
	
	#ifndef glGenFramebuffers
		#define glGenFramebuffers                        pglGenFramebuffers
	#endif
	#ifndef glDeleteFramebuffers
		#define glDeleteFramebuffers                     pglDeleteFramebuffers
	#endif
	#ifndef glBindFramebuffer
		#define glBindFramebuffer                        pglBindFramebuffer
	#endif
	#ifndef glCheckFramebufferStatus
		#define glCheckFramebufferStatus                 pglCheckFramebufferStatus
	#endif
	#ifndef glGetFramebufferAttachmentParameteriv
		#define glGetFramebufferAttachmentParameteriv    pglGetFramebufferAttachmentParameteriv
	#endif
	#ifndef glGenerateMipmap
		#define glGenerateMipmap                         pglGenerateMipmap
	#endif
	#ifndef glFramebufferTexture2D
		#define glFramebufferTexture2D                   pglFramebufferTexture2D
	#endif
	#ifndef glFramebufferRenderbuffer
		#define glFramebufferRenderbuffer                pglFramebufferRenderbuffer
	#endif

	#ifndef glGenRenderbuffers
		#define glGenRenderbuffers                       pglGenRenderbuffers
	#endif
	#ifndef glDeleteRenderbuffers
		#define glDeleteRenderbuffers                    pglDeleteRenderbuffers
	#endif
	#ifndef glBindRenderbuffer
		#define glBindRenderbuffer                       pglBindRenderbuffer
	#endif
	#ifndef glRenderbufferStorage
		#define glRenderbufferStorage                    pglRenderbufferStorage
	#endif
	#ifndef glGetRenderbufferParameteriv
		#define glGetRenderbufferParameteriv             pglGetRenderbufferParameteriv
	#endif
	#ifndef glIsRenderBuffer
		#define glIsRenderbuffer                         pglIsRenderbuffer
	#endif

	#ifndef glCreateProgram
		#define glCreateProgram							 pglCreateProgram
	#endif
	#ifndef glCreateShader
		#define glCreateShader							 pglCreateShader
	#endif
	#ifndef glShaderSource
		#define glShaderSource							 pglShaderSource
	#endif
	#ifndef glCompileShader
		#define glCompileShader							 pglCompileShader
	#endif
	#ifndef glGetObjectParameterivARB
		#define glGetObjectParameterivARB				 pglGetObjectParameterivARB
	#endif
	#ifndef glAttachShader
		#define glAttachShader							 pglAttachShader
	#endif
	#ifndef glLinkProgram
		#define glLinkProgram							 pglLinkProgram
	#endif	
	#ifndef glGetShaderiv
		#define glGetShaderiv							 pglGetShaderiv
	#endif
	#ifndef glGetProgramivARB
		#define glGetProgramivARB						 pglGetProgramivARB
	#endif
	#ifndef glUseProgram
		#define glUseProgram							 pglUseProgram
	#endif
	#ifndef glGetUniformLocation
		#define glGetUniformLocation					 pglGetUniformLocation
	#endif
	#ifndef glUniform2f
		#define glUniform2f								 pglUniform2f
	#endif
	#ifndef glUniform1i
		#define glUniform1i								 pglUniform1i
	#endif

	#ifndef glBindBuffer
		#define glBindBuffer							 pglBindBuffer
	#endif
	#ifndef glGenBuffers
		#define glGenBuffers							 pglGenBuffers
	#endif
	#ifndef glBufferData
		#define glBufferData							 pglBufferData
	#endif

	#ifndef glActiveTexture
		#define glActiveTexture							 pglActiveTexture
	#endif

#endif
};

}// End namespace

#endif