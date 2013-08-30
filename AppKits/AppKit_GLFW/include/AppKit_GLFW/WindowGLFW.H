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

#ifndef WINDOWGLFW_H
#define WINDOWGLFW_H


#include "GL/glew.h"
#ifdef _WIN32
#include "GL/wglew.h"
#else
#include "GL/glxew.h"
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