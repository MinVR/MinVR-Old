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

#include "AppKit_GLFW/WindowGLFW.H"

namespace MinVR {

#define MAX_AFFINITY_GPUS 16

using namespace std;

std::map<GLFWwindow*, WindowGLFW*> WindowGLFW::pointerToObjectMap(WindowGLFW::initPointerToObjectMap()); 

std::map<GLFWwindow*, WindowGLFW*> WindowGLFW::initPointerToObjectMap()
{
	std::map<GLFWwindow*, WindowGLFW*> map;
	return map;
}

WindowGLFW::WindowGLFW(WindowSettingsRef settings, std::vector<AbstractCameraRef> cameras) : AbstractWindow(settings, cameras)
{
	firstTime = true;

	initGLEW();

	// If we have access to the nvidia affinity extension, discover the gpus on the system
	// and find which one corresponds with this window size and position.
#ifdef _MSC_VER
	if (settings->useGPUAffinity) {
		GPU_DEVICE gpus[MAX_AFFINITY_GPUS];
		if (WGLEW_NV_gpu_affinity) {
			int numGPUs = 0;
			for( UINT gpu = 0; true; ++gpu ) {
				HGPUNV hGPU = 0;
				if( !wglEnumGpusNV( gpu, &hGPU )) {
					break;
				}
				wglEnumGpuDevicesNV( hGPU, 0, &gpus[gpu] );
				numGPUs++;
			}

			int currentGPU = -1;
			for(int j=0; j < numGPUs; j++) {
				if (settings->xPos >= gpus[j].rcVirtualScreen.left && (settings->xPos+settings->width) <= gpus[j].rcVirtualScreen.right &&
					settings->yPos >= gpus[j].rcVirtualScreen.top && (settings->yPos+settings->height) <= gpus[j].rcVirtualScreen.bottom) {
						currentGPU = j;
						break;
				}
			}

			glfwWindowHint(GLFW_AFFINITY_GPU, currentGPU);
		}
	}
#endif
	glfwWindowHint(GLFW_ALPHA_BITS, settings->alphaBits);
	glfwWindowHint(GLFW_DEPTH_BITS, settings->depthBits);
	glfwWindowHint(GLFW_DECORATED, settings->framed);
	glfwWindowHint(GLFW_SAMPLES, settings->msaaSamples);
	glfwWindowHint(GLFW_RESIZABLE, settings->resizable);
	glfwWindowHint(GLFW_RED_BITS, settings->rgbBits);
	glfwWindowHint(GLFW_GREEN_BITS, settings->rgbBits);
	glfwWindowHint(GLFW_BLUE_BITS, settings->rgbBits);
	glfwWindowHint(GLFW_STENCIL_BITS, settings->stencilBits);
	glfwWindowHint(GLFW_STEREO, settings->stereo);
	glfwWindowHint(GLFW_VISIBLE, settings->visible);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, settings->useDebugContext);

	if (settings->fullScreen) {
		// Find the monitor that most closely matches the position
		int count;
		GLFWmonitor** monitors = glfwGetMonitors(&count);
		int closestInd = 0;
		int closestXPos = 0;
		int closestYPos = 0;
		float closestDist = 100000;
		int xPos, yPos;
		for(int i=0; i < count; i++) {
			glfwGetMonitorPos(monitors[i], &xPos, &yPos);
			float curDist = pow((float)(closestXPos - xPos), 2.0f) + pow((float)(closestYPos - yPos), 2.0f);
			if (curDist <= closestDist) {
				closestInd = i;
				closestXPos = xPos;
				closestYPos = yPos;
				closestDist = curDist;
			}
		}
		_window = glfwCreateWindow(settings->width, settings->height, settings->windowTitle.c_str(), monitors[closestInd], NULL);
	}
	else {
		_window = glfwCreateWindow(settings->width, settings->height, settings->windowTitle.c_str(), NULL, NULL);
	}
	if (!_window) {
		glfwTerminate();
		BOOST_ASSERT_MSG(false, "Unable to create new GLFW window");
	}

	WindowGLFW::pointerToObjectMap.insert(std::pair<GLFWwindow*, WindowGLFW*>(_window, this));

	glfwSetWindowPos(_window, settings->xPos, settings->yPos);

	_width = settings->width;
	_height = settings->height;
	glfwSetWindowSizeCallback(_window, &window_size_callback);

	_xPos = settings->xPos;
	_yPos = settings->yPos;
	glfwSetWindowPosCallback(_window, &window_pos_callback);

	glfwShowWindow(_window);
}

WindowGLFW::~WindowGLFW()
{
	WindowGLFW::pointerToObjectMap.erase(WindowGLFW::pointerToObjectMap.find(_window));
}


void WindowGLFW::initGLEW()
{
	// Initialize glew
	// Requires that a context exists and is current before it will work, so we create a temporary one here
	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
	GLFWwindow* tempWin = glfwCreateWindow(200, 200, "Temporary", NULL, NULL);
	glfwMakeContextCurrent(tempWin);
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		BOOST_ASSERT_MSG(false, "Unable to initialize GLEW");
	}
	glfwMakeContextCurrent(NULL);
	glfwDestroyWindow(tempWin);
}

void WindowGLFW::pollForInput(std::vector<EventRef> &events)
{
	for(auto event_it=_currentEvents.begin(); event_it!=_currentEvents.end(); ++event_it) {
		events.push_back(*event_it);
	}
	_currentEvents.clear();
	glfwPollEvents();
}

void WindowGLFW::swapBuffers()
{
	glfwSwapBuffers(_window);
}

void WindowGLFW::makeContextCurrent()
{
	glfwMakeContextCurrent(_window);

	if (firstTime) {
		initDebugCallback();
		firstTime = false;
	}

	GLenum err;
	if((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "openGL ERROR  make context current: "<<err<<std::endl;
	}
}

void WindowGLFW::releaseContext()
{
	glfwMakeContextCurrent(NULL);
}

int WindowGLFW::getWidth()
{
	//TODO: This returns the size of the window, should we be returning the size of the framebuffer instead?
	// http://www.glfw.org/docs/latest/window.html has more details about how to do this and that they can be different

	return _width;
}

int WindowGLFW::getHeight()
{
	//TODO: This returns the size of the window, should we be returning the size of the framebuffer instead?
	// http://www.glfw.org/docs/latest/window.html has more details about how to do this and that they can be different

	return _height;
}

int WindowGLFW::getXPos()
{
	return _xPos;
}

int WindowGLFW::getYPos()
{
	return _yPos;
}

void WindowGLFW::setSize(int width, int height, bool actuallySet)
{
	_width = width;
	_height = height;

	if (actuallySet) {
		glfwSetWindowSize(_window, _width, _height);
	}
}

void WindowGLFW::setPosition(int xPos, int yPos, bool actuallySet)
{
	_xPos = xPos;
	_yPos = yPos;

	if (actuallySet) {
		glfwSetWindowPos(_window, _xPos, _yPos);
	}
}

void WindowGLFW::window_size_callback(GLFWwindow* window, int width, int height)
{
	WindowGLFW* obj = (*(WindowGLFW::pointerToObjectMap.find(window))).second;
	obj->setSize(width, height, false);
}

void WindowGLFW::window_pos_callback(GLFWwindow* window, int xpos, int ypos)
{
	WindowGLFW* obj = (*(WindowGLFW::pointerToObjectMap.find(window))).second;
	obj->setPosition(xpos, ypos, false);
}

GLFWwindow* WindowGLFW::getWindowPtr()
{
	return _window;
}

void WindowGLFW::appendEvent(EventRef newEvent)
{
	_currentEvents.push_back(newEvent);
}

void WindowGLFW::setCursorPosition(double x, double y)
{
	_cursorPosition.x = (float)x;
	_cursorPosition.y = (float)y;
}

glm::vec2 WindowGLFW::getCursorPosition()
{
	return _cursorPosition;
}

void WindowGLFW::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	string name = getButtonName(button);

    if (mods) {
		name = name + "_" + getModsName(mods);
	}

    name = name + "_" + getActionName(action);

	WindowGLFW* obj = (*(WindowGLFW::pointerToObjectMap.find(window))).second;
	EventRef newEvent(new Event(name, obj->getCursorPosition(), WindowRef(obj)));
	obj->appendEvent(newEvent);
}

void WindowGLFW::cursor_position_callback(GLFWwindow* window, double x, double y)
{
	string name = "mouse_pointer";
    WindowGLFW* obj = (*(WindowGLFW::pointerToObjectMap.find(window))).second;
	obj->setCursorPosition(x, y);
	EventRef newEvent(new Event(name, obj->getCursorPosition(), WindowRef(obj)));
	obj->appendEvent(newEvent);
}

void WindowGLFW::cursor_enter_callback(GLFWwindow* window, int entered)
{
	string name = "mouse_pointer_" + entered ? "entered" : "left";
    WindowGLFW* obj = (*(WindowGLFW::pointerToObjectMap.find(window))).second;
	EventRef newEvent(new Event(name, WindowRef(obj)));
	obj->appendEvent(newEvent);
}

void WindowGLFW::scroll_callback(GLFWwindow* window, double x, double y)
{
	string name = "mouse_scroll";
	WindowGLFW* obj = (*(WindowGLFW::pointerToObjectMap.find(window))).second;
	EventRef newEvent(new Event(name, glm::vec2(x, y), WindowRef(obj)));
	obj->appendEvent(newEvent);
}

void WindowGLFW::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    string name = "kbd_" + getKeyName(key);
	
	if (mods) {
		name = name + "_" + getModsName(mods);
	}

	string value = getKeyValue(key, mods);

    name = name + "_" + getActionName(action);

	WindowGLFW* obj = (*(WindowGLFW::pointerToObjectMap.find(window))).second;
	EventRef newEvent(new Event(name, value, WindowRef(obj)));
	obj->appendEvent(newEvent);
}

string WindowGLFW::getKeyName(int key)
{
    switch (key)
    {
        // Printable keys
        case GLFW_KEY_A:            return "A";
        case GLFW_KEY_B:            return "B";
        case GLFW_KEY_C:            return "C";
        case GLFW_KEY_D:            return "D";
        case GLFW_KEY_E:            return "E";
        case GLFW_KEY_F:            return "F";
        case GLFW_KEY_G:            return "G";
        case GLFW_KEY_H:            return "H";
        case GLFW_KEY_I:            return "I";
        case GLFW_KEY_J:            return "J";
        case GLFW_KEY_K:            return "K";
        case GLFW_KEY_L:            return "L";
        case GLFW_KEY_M:            return "M";
        case GLFW_KEY_N:            return "N";
        case GLFW_KEY_O:            return "O";
        case GLFW_KEY_P:            return "P";
        case GLFW_KEY_Q:            return "Q";
        case GLFW_KEY_R:            return "R";
        case GLFW_KEY_S:            return "S";
        case GLFW_KEY_T:            return "T";
        case GLFW_KEY_U:            return "U";
        case GLFW_KEY_V:            return "V";
        case GLFW_KEY_W:            return "W";
        case GLFW_KEY_X:            return "X";
        case GLFW_KEY_Y:            return "Y";
        case GLFW_KEY_Z:            return "Z";
        case GLFW_KEY_1:            return "1";
        case GLFW_KEY_2:            return "2";
        case GLFW_KEY_3:            return "3";
        case GLFW_KEY_4:            return "4";
        case GLFW_KEY_5:            return "5";
        case GLFW_KEY_6:            return "6";
        case GLFW_KEY_7:            return "7";
        case GLFW_KEY_8:            return "8";
        case GLFW_KEY_9:            return "9";
        case GLFW_KEY_0:            return "0";
        case GLFW_KEY_SPACE:        return "SPACE";
        case GLFW_KEY_MINUS:        return "MINUS";
        case GLFW_KEY_EQUAL:        return "EQUAL";
        case GLFW_KEY_LEFT_BRACKET: return "LEFT_BRACKET";
        case GLFW_KEY_RIGHT_BRACKET: return "RIGHT_BRACKET";
        case GLFW_KEY_BACKSLASH:    return "BACKSLASH";
        case GLFW_KEY_SEMICOLON:    return "SEMICOLON";
        case GLFW_KEY_APOSTROPHE:   return "APOSTROPHE";
        case GLFW_KEY_GRAVE_ACCENT: return "GRAVE_ACCENT";
        case GLFW_KEY_COMMA:        return "COMMA";
        case GLFW_KEY_PERIOD:       return "PERIOD";
        case GLFW_KEY_SLASH:        return "SLASH";
        case GLFW_KEY_WORLD_1:      return "WORLD1";
        case GLFW_KEY_WORLD_2:      return "WORLD2";

        // Function keys
        case GLFW_KEY_ESCAPE:       return "ESC";
        case GLFW_KEY_F1:           return "F1";
        case GLFW_KEY_F2:           return "F2";
        case GLFW_KEY_F3:           return "F3";
        case GLFW_KEY_F4:           return "F4";
        case GLFW_KEY_F5:           return "F5";
        case GLFW_KEY_F6:           return "F6";
        case GLFW_KEY_F7:           return "F7";
        case GLFW_KEY_F8:           return "F8";
        case GLFW_KEY_F9:           return "F9";
        case GLFW_KEY_F10:          return "F10";
        case GLFW_KEY_F11:          return "F11";
        case GLFW_KEY_F12:          return "F12";
        case GLFW_KEY_F13:          return "F13";
        case GLFW_KEY_F14:          return "F14";
        case GLFW_KEY_F15:          return "F15";
        case GLFW_KEY_F16:          return "F16";
        case GLFW_KEY_F17:          return "F17";
        case GLFW_KEY_F18:          return "F18";
        case GLFW_KEY_F19:          return "F19";
        case GLFW_KEY_F20:          return "F20";
        case GLFW_KEY_F21:          return "F21";
        case GLFW_KEY_F22:          return "F22";
        case GLFW_KEY_F23:          return "F23";
        case GLFW_KEY_F24:          return "F24";
        case GLFW_KEY_F25:          return "F25";
        case GLFW_KEY_UP:           return "UP";
        case GLFW_KEY_DOWN:         return "DOWN";
        case GLFW_KEY_LEFT:         return "LEFT";
        case GLFW_KEY_RIGHT:        return "RIGHT";
        case GLFW_KEY_LEFT_SHIFT:   return "LEFT_SHIFT";
        case GLFW_KEY_RIGHT_SHIFT:  return "RIGHT_SHIFT";
        case GLFW_KEY_LEFT_CONTROL: return "LEFT_CONTROL";
        case GLFW_KEY_RIGHT_CONTROL: return "RIGHT_CONTROL";
        case GLFW_KEY_LEFT_ALT:     return "LEFT_ALT";
        case GLFW_KEY_RIGHT_ALT:    return "RIGHT_ALT";
        case GLFW_KEY_TAB:          return "TAB";
        case GLFW_KEY_ENTER:        return "ENTER";
        case GLFW_KEY_BACKSPACE:    return "BACKSPACE";
        case GLFW_KEY_INSERT:       return "INSERT";
        case GLFW_KEY_DELETE:       return "DELETE";
        case GLFW_KEY_PAGE_UP:      return "PAGEUP";
        case GLFW_KEY_PAGE_DOWN:    return "PAGEDOWN";
        case GLFW_KEY_HOME:         return "HOME";
        case GLFW_KEY_END:          return "END";
        case GLFW_KEY_KP_0:         return "KEYPAD0";
        case GLFW_KEY_KP_1:         return "KEYPAD1";
        case GLFW_KEY_KP_2:         return "KEYPAD2";
        case GLFW_KEY_KP_3:         return "KEYPAD3";
        case GLFW_KEY_KP_4:         return "KEYPAD4";
        case GLFW_KEY_KP_5:         return "KEYPAD5";
        case GLFW_KEY_KP_6:         return "KEYPAD6";
        case GLFW_KEY_KP_7:         return "KEYPAD7";
        case GLFW_KEY_KP_8:         return "KEYPAD8";
        case GLFW_KEY_KP_9:         return "KEYPAD9";
        case GLFW_KEY_KP_DIVIDE:    return "KEYPAD_DIVIDE";
        case GLFW_KEY_KP_MULTIPLY:  return "KEYPAD_MULTIPLY";
        case GLFW_KEY_KP_SUBTRACT:  return "KEYPAD_SUBTRACT";
        case GLFW_KEY_KP_ADD:       return "KEYPAD_ADD";
        case GLFW_KEY_KP_DECIMAL:   return "KEYPAD_DECIMAL";
        case GLFW_KEY_KP_EQUAL:     return "KEYPAD_EQUAL";
        case GLFW_KEY_KP_ENTER:     return "KEYPAD_ENTER";
        case GLFW_KEY_PRINT_SCREEN: return "PRINT_SCREEN";
        case GLFW_KEY_NUM_LOCK:     return "NUM_LOCK";
        case GLFW_KEY_CAPS_LOCK:    return "CAPS_LOCK";
        case GLFW_KEY_SCROLL_LOCK:  return "SCROLL_LOCK";
        case GLFW_KEY_PAUSE:        return "PAUSE";
        case GLFW_KEY_LEFT_SUPER:   return "LEFT_SUPER";
        case GLFW_KEY_RIGHT_SUPER:  return "RIGHT_SUPER";
        case GLFW_KEY_MENU:         return "MENU";
        case GLFW_KEY_UNKNOWN:      return "UNKNOWN";

        default:                    return "";
    }
}

string WindowGLFW::getKeyValue(int key, int mods)
{
	string value = "";
	switch (key)
    {
        // Printable keys
        case GLFW_KEY_A:            value = "a"; break;
        case GLFW_KEY_B:            value = "b"; break;
        case GLFW_KEY_C:            value = "c"; break;
        case GLFW_KEY_D:            value = "d"; break;
        case GLFW_KEY_E:            value = "e"; break;
        case GLFW_KEY_F:            value = "f"; break;
        case GLFW_KEY_G:            value = "g"; break;
        case GLFW_KEY_H:            value = "h"; break;
        case GLFW_KEY_I:            value = "i"; break;
        case GLFW_KEY_J:            value = "j"; break;
        case GLFW_KEY_K:            value = "k"; break;
        case GLFW_KEY_L:            value = "l"; break;
        case GLFW_KEY_M:            value = "m"; break;
        case GLFW_KEY_N:            value = "n"; break;
        case GLFW_KEY_O:            value = "o"; break;
        case GLFW_KEY_P:            value = "p"; break;
        case GLFW_KEY_Q:            value = "q"; break;
        case GLFW_KEY_R:            value = "r"; break;
        case GLFW_KEY_S:            value = "s"; break;
        case GLFW_KEY_T:            value = "t"; break;
        case GLFW_KEY_U:            value = "u"; break;
        case GLFW_KEY_V:            value = "v"; break;
        case GLFW_KEY_W:            value = "w"; break;
        case GLFW_KEY_X:            value = "x"; break;
        case GLFW_KEY_Y:            value = "y"; break;
        case GLFW_KEY_Z:            value = "z"; break;

		default:					value = ""; break;
	}

	if (value != "") {
		if (mods & GLFW_MOD_SHIFT)
		{
			boost::to_upper(value);
		}
	}
	else {

		switch(key)
		{
			case GLFW_KEY_1:            value = "1"; break;
			case GLFW_KEY_2:            value = "2"; break;
			case GLFW_KEY_3:            value = "3"; break;
			case GLFW_KEY_4:            value = "4"; break;
			case GLFW_KEY_5:            value = "5"; break;
			case GLFW_KEY_6:            value = "6"; break;
			case GLFW_KEY_7:            value = "7"; break;
			case GLFW_KEY_8:            value =  "8"; break;
			case GLFW_KEY_9:            value = "9"; break;
			case GLFW_KEY_0:            value = "0"; break;
			case GLFW_KEY_SPACE:        value = " "; break;
			case GLFW_KEY_MINUS:        value = "-"; break;
			case GLFW_KEY_EQUAL:        value = "="; break;
			case GLFW_KEY_LEFT_BRACKET: value = "]"; break;
			case GLFW_KEY_RIGHT_BRACKET: value = "["; break;
			case GLFW_KEY_BACKSLASH:    value = "\\"; break;
			case GLFW_KEY_SEMICOLON:    value = ";"; break;
			case GLFW_KEY_APOSTROPHE:   value = "'"; break;
			case GLFW_KEY_GRAVE_ACCENT: value = "`"; break;
			case GLFW_KEY_COMMA:        value = ","; break;
			case GLFW_KEY_PERIOD:       value = "."; break;
			case GLFW_KEY_SLASH:        value = "/"; break;
			case GLFW_KEY_TAB:          value = "\t"; break;
			case GLFW_KEY_ENTER:        value = "\n"; break;
			case GLFW_KEY_KP_0:         value = "0"; break;
			case GLFW_KEY_KP_1:         value = "1"; break;
			case GLFW_KEY_KP_2:         value = "2"; break;
			case GLFW_KEY_KP_3:         value = "3"; break;
			case GLFW_KEY_KP_4:         value = "4"; break;
			case GLFW_KEY_KP_5:         value = "5"; break;
			case GLFW_KEY_KP_6:         value = "6"; break;
			case GLFW_KEY_KP_7:         value = "7"; break;
			case GLFW_KEY_KP_8:         value = "8"; break;
			case GLFW_KEY_KP_9:         value = "9"; break;
			case GLFW_KEY_KP_DIVIDE:    value = "/"; break;
			case GLFW_KEY_KP_MULTIPLY:  value = "*"; break;
			case GLFW_KEY_KP_SUBTRACT:  value = "-"; break;
			case GLFW_KEY_KP_ADD:       value = "+"; break;
			case GLFW_KEY_KP_DECIMAL:   value = "."; break;
			case GLFW_KEY_KP_EQUAL:     value = "="; break;
			case GLFW_KEY_KP_ENTER:     value = "\n"; break;
		
			default:                    break;
		}
	}

	return value;
}

string WindowGLFW::getActionName(int action)
{
    switch (action)
    {
        case GLFW_PRESS:
            return "down";
        case GLFW_RELEASE:
            return "up";
        case GLFW_REPEAT:
            return "repeat";
    }

    return "caused unknown action";
}

string WindowGLFW::getButtonName(int button)
{
    switch (button)
    {
        case GLFW_MOUSE_BUTTON_LEFT:
            return "mouse_btn_left";
        case GLFW_MOUSE_BUTTON_RIGHT:
            return "mouse_btn_right";
        case GLFW_MOUSE_BUTTON_MIDDLE:
            return "mouse_btn_middle";
		default:
			std::stringstream ss;
			ss << "mouse_btn_"<<button;
			return ss.str();
    }

    return "";
}

string WindowGLFW::getModsName(int mods)
{
	string modName = "";
    if (mods & GLFW_MOD_SHIFT)
        modName += "SHIFT";
    if (mods & GLFW_MOD_CONTROL)
        modName += "CTRL";
    if (mods & GLFW_MOD_ALT)
        modName += "ALT";
    if (mods & GLFW_MOD_SUPER)
        modName += "SUPER";

    return modName;
}
    

#ifndef _MSC_VER
#define _snprintf snprintf
#endif

void WindowGLFW::formatDebugOutputARB(char outStr[], size_t outStrSize, GLenum source, GLenum type, GLuint id, GLenum severity, const char *msg)
{
    char sourceStr[32];
    const char *sourceFmt = "UNDEFINED(0x%04X)";
    switch(source)
    {
    case GL_DEBUG_SOURCE_API_ARB:             sourceFmt = "API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:   sourceFmt = "WINDOW_SYSTEM"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB: sourceFmt = "SHADER_COMPILER"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY_ARB:     sourceFmt = "THIRD_PARTY"; break;
    case GL_DEBUG_SOURCE_APPLICATION_ARB:     sourceFmt = "APPLICATION"; break;
    case GL_DEBUG_SOURCE_OTHER_ARB:           sourceFmt = "OTHER"; break;
    }

    _snprintf(sourceStr, 32, sourceFmt, source);
 
    char typeStr[32];
    const char *typeFmt = "UNDEFINED(0x%04X)";
    switch(type)
    {
    case GL_DEBUG_TYPE_ERROR_ARB:               typeFmt = "ERROR"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB: typeFmt = "DEPRECATED_BEHAVIOR"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:  typeFmt = "UNDEFINED_BEHAVIOR"; break;
    case GL_DEBUG_TYPE_PORTABILITY_ARB:         typeFmt = "PORTABILITY"; break;
    case GL_DEBUG_TYPE_PERFORMANCE_ARB:         typeFmt = "PERFORMANCE"; break;
    case GL_DEBUG_TYPE_OTHER_ARB:               typeFmt = "OTHER"; break;
    }
    _snprintf(typeStr, 32, typeFmt, type);

 
    char severityStr[32];
    const char *severityFmt = "UNDEFINED";
    switch(severity)
    {
    case GL_DEBUG_SEVERITY_HIGH_ARB:   severityFmt = "HIGH";   break;
    case GL_DEBUG_SEVERITY_MEDIUM_ARB: severityFmt = "MEDIUM"; break;
    case GL_DEBUG_SEVERITY_LOW_ARB:    severityFmt = "LOW"; break;
    }

    _snprintf(severityStr, 32, severityFmt, severity);
 
    _snprintf(outStr, outStrSize, "OpenGL: %s [source=%s type=%s severity=%s id=%d]", msg, sourceStr, typeStr, severityStr, id);
}

void WindowGLFW::debugCallbackARB(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, GLvoid *userParam)
{
    (void)length;
    FILE *outFile = (FILE*)userParam;
    char finalMessage[256];
    formatDebugOutputARB(finalMessage, 256, source, type, id, severity, message);
    fprintf(outFile, "%s\n", finalMessage);

}

void WindowGLFW::formatDebugOutputAMD(char outStr[], size_t outStrSize, GLenum category, GLuint id, GLenum severity, const char *msg)
{
    char categoryStr[32];
    const char *categoryFmt = "UNDEFINED(0x%04X)";
    switch(category)
    {
    case GL_DEBUG_CATEGORY_API_ERROR_AMD:          categoryFmt = "API_ERROR"; break;
    case GL_DEBUG_CATEGORY_WINDOW_SYSTEM_AMD:      categoryFmt = "WINDOW_SYSTEM"; break;
    case GL_DEBUG_CATEGORY_DEPRECATION_AMD:        categoryFmt = "DEPRECATION"; break;
    case GL_DEBUG_CATEGORY_UNDEFINED_BEHAVIOR_AMD: categoryFmt = "UNDEFINED_BEHAVIOR"; break;
    case GL_DEBUG_CATEGORY_PERFORMANCE_AMD:        categoryFmt = "PERFORMANCE"; break;
    case GL_DEBUG_CATEGORY_SHADER_COMPILER_AMD:    categoryFmt = "SHADER_COMPILER"; break;
    case GL_DEBUG_CATEGORY_APPLICATION_AMD:        categoryFmt = "APPLICATION"; break;
    case GL_DEBUG_CATEGORY_OTHER_AMD:              categoryFmt = "OTHER"; break;
    }
    _snprintf(categoryStr, 32, categoryFmt, category);

    char severityStr[32];
    const char *severityFmt = "UNDEFINED";
    switch(severity)
    {
    case GL_DEBUG_SEVERITY_HIGH_AMD:   severityFmt = "HIGH";   break;
    case GL_DEBUG_SEVERITY_MEDIUM_AMD: severityFmt = "MEDIUM"; break;
    case GL_DEBUG_SEVERITY_LOW_AMD:    severityFmt = "LOW";    break;
    }
    _snprintf(severityStr, 32, severityFmt, severity);

    _snprintf(outStr, outStrSize, "OpenGL: %s [category=%s severity=%s id=%d]",
        msg, categoryStr, severityStr, id);
}

void WindowGLFW::debugCallbackAMD(GLuint id, GLenum category, GLenum severity, GLsizei length, const GLchar *message, GLvoid *userParam)
{
	//cout << id << " " << category << " " << severity << " l" << length << " msg:" << message << endl;
	
    (void)length;
    FILE *outFile = (FILE*)userParam;
    char finalMsg[256];
    formatDebugOutputAMD(finalMsg, 256, category, id, severity, message);
    fprintf(outFile, "%s\n", finalMsg);
}

void WindowGLFW::initDebugCallback()
{
	if (_settings->useDebugContext) {
		if (glewIsSupported("GL_ARB_Debug_output"))
		{
			glDebugMessageCallbackARB(&debugCallbackARB, stdout);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
		}
		else if (glewIsSupported("GL_AMD_debug_output"))
		{
			glDebugMessageCallbackAMD(&debugCallbackAMD, stdout);
			//glDebugMessageInsertAMD(GL_DEBUG_CATEGORY_APPLICATION_AMD, GL_DEBUG_SEVERITY_HIGH_AMD, 1, 4, "test");
		}
	}
}

} // end namespace