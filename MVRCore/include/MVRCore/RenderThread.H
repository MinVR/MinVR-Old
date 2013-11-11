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

#if defined(WIN32)
#define NOMINMAX
#include <windows.h>
#include <GL/gl.h>
#elif defined(__APPLE__)
#include <OpenGL/OpenGL.h>
#include <OpenGL/glu.h>
#else
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#include "GL/glext.h"


namespace MinVR {

// forward declaration
class AbstractMVREngine;

typedef std::shared_ptr<class RenderThread> RenderThreadRef;

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
	static size_t numRenderingThreads;
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
	GLuint _stereoProgram;
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
