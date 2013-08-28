#ifndef GLFWDEMOAPP_H
#define GLFWDEMOAPP_H

#include "GL/glew.h"
#include "MVRCore/AbstractMVRApp.H"
#include "MVRCore/AbstractCamera.H"
#include "MVRCore/AbstractWindow.H"
#include "G3DLite/G3DLite.h"
#include "MVRCore/Event.H"
#include <GLFW/glfw3.h>
#include <boost/thread.hpp>

class GLFWDemoApp : public MinVR::AbstractMVRApp
{
public:
	GLFWDemoApp();
	~GLFWDemoApp();

	void doUserInputAndPreDrawComputation(const G3DLite::Array<MinVR::EventRef> &events, double synchronizedTime);
	void initializeContextSpecificVars(int threadId, MinVR::WindowRef window);
	void drawGraphics(int threadId, MinVR::AbstractCameraRef camera, MinVR::WindowRef window);

private:
	void initGL();
	void initVBO();
	void initLights();

	boost::thread_specific_ptr<GLuint> _vboId;
};

#endif