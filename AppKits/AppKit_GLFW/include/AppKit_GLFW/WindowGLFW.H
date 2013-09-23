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

#ifndef WINDOWGLFW_H
#define WINDOWGLFW_H


#include "GL/glew.h"
#ifdef _WIN32
#include "GL/wglew.h"
#elif (!defined(__APPLE__))
#include "GL/glxew.h"
#endif

#if !defined(WIN32)
#define CALLBACK
#endif

#include <GLFW/glfw3.h>

#include "MVRCore/AbstractWindow.H"
#include "MVRCore/Event.H"
#include <map>
#include <boost/algorithm/string.hpp>
#include <string>
#include <glm/glm.hpp>

namespace MinVR {

class WindowGLFW : public AbstractWindow
{
public:
	WindowGLFW(WindowSettingsRef settings, std::vector<AbstractCameraRef> cameras);
	~WindowGLFW();

	void pollForInput(std::vector<EventRef> &events);
	void swapBuffers();
	void makeContextCurrent();
	void releaseContext();
	int getWidth();
	int getHeight();
	int getXPos();
	int getYPos();
	GLFWwindow* getWindowPtr();

	/** This method is called from the window_size_callback to update the _width and _height member
		variables. It can also be used to change the window size if actuallySet is true.
	*/
	void setSize(int width, int height, bool actuallySet);

	/** This method is called from the window_pos_callback to update the _xPos and _yPos member
		variables. It can also be used to change the window position if actuallySet is true.
	*/
	void setPosition(int xPos, int yPos, bool actuallySet);

	/** This method is called from the event callback methods to add an event to _currentEvents
		for the next pollForInput to process
	*/
	void appendEvent(EventRef newEvent);

	/** This method is called from the cursor position callback to keep track of the current cursor
		position to send along with mouse events
	*/
	void setCursorPosition(double x, double y);

	/** Gets the most recently updated cursor position in this window
	*/
	glm::vec2 getCursorPosition();

	/** This uglyness is caused because glfw is a C library so its callback functions for
		key presses, size changes, etc. must be static or regular functions. Unfortunately,
		we need to access the member variables in the callback functions, so we use this
		hash table as a way to find the object and access the members.
	*/
	static std::map<GLFWwindow*, WindowGLFW*> pointerToObjectMap;
	static std::map<GLFWwindow*, WindowGLFW*> initPointerToObjectMap();

	static void window_size_callback(GLFWwindow* window, int width, int height);
	static void window_pos_callback(GLFWwindow* window, int xpos, int ypos);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_position_callback(GLFWwindow* window, double x, double y);
	static void cursor_enter_callback(GLFWwindow* window, int entered);
	static void scroll_callback(GLFWwindow* window, double x, double y);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
	GLFWwindow* _window;
	int _width;
	int _height;
	int _xPos;
	int _yPos;
	std::vector<EventRef> _currentEvents;
	glm::vec2 _cursorPosition;

	void initGLEW();

	// Keypress helper methods
	static std::string getKeyName(int key);
	static std::string getKeyValue(int key, int mods);
	static std::string getActionName(int action);
	static std::string getButtonName(int button);
	static std::string getModsName(int mods);

	void initDebugCallback();
	bool firstTime;

	static void formatDebugOutputARB(char outStr[], size_t outStrSize, GLenum source, GLenum type, GLuint id, GLenum severity, const char *msg);
	static void formatDebugOutputAMD(char outStr[], size_t outStrSize, GLenum category, GLuint id, GLenum severity, const char *msg);
	static void CALLBACK debugCallbackARB(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, GLvoid *userParam);
	static void CALLBACK debugCallbackAMD(GLuint id, GLenum category, GLenum severity, GLsizei length, const GLchar *message, GLvoid *userParam);
};

} // end namespace

#endif
