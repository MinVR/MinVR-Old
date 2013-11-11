#ifndef GLFWDEMOAPP_H
#define GLFWDEMOAPP_H

#include "GL/glew.h"
#include "MVRCore/AbstractMVRApp.H"
#include "MVRCore/AbstractCamera.H"
#include "MVRCore/AbstractWindow.H"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "MVRCore/Event.H"
#include <GLFW/glfw3.h>
#include <boost/thread.hpp>
#include <vector>

class GLFWDemoApp : public MinVR::AbstractMVRApp
{
public:
	GLFWDemoApp();
	~GLFWDemoApp();

	void doUserInputAndPreDrawComputation(const std::vector<MinVR::EventRef> &events, double synchronizedTime);
	void initializeContextSpecificVars(int threadId, MinVR::WindowRef window);
	void postInitialization();
	void drawGraphics(int threadId, MinVR::AbstractCameraRef camera, MinVR::WindowRef window);

private:
	void initGL();
	void initVBO();
	void initLights();

	boost::thread_specific_ptr<GLuint> _vboId;
};

#endif