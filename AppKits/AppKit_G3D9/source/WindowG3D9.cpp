//========================================================================
// MinVR - AppKit G3D9
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

#include "AppKit_G3D9/WindowG3D9.H"

#include <GLFW/glfw3.h>

namespace MinVR {

using namespace std;

boost::mutex WindowG3D9::_RDMUTEX;

WindowG3D9::WindowG3D9(WindowSettingsRef settings, std::vector<AbstractCameraRef> cameras) : AbstractWindow(settings, cameras)
{
	_renderDevice = nullptr;

	G3D::OSWindow::Settings osWinSettings = G3D::OSWindow::Settings();
	osWinSettings.allowMaximize = settings->resizable;
	osWinSettings.alphaBits = settings->alphaBits;
	osWinSettings.asynchronous = true;
	osWinSettings.caption = settings->windowTitle;
	osWinSettings.center = false;
	osWinSettings.debugContext = settings->useDebugContext;
	osWinSettings.depthBits = settings->depthBits;
	osWinSettings.framed = settings->framed;
	osWinSettings.fullScreen = settings->fullScreen;
	osWinSettings.height = settings->height;
	osWinSettings.msaaSamples = settings->msaaSamples;
	osWinSettings.resizable = settings->resizable;
	osWinSettings.rgbBits = settings->rgbBits;
	osWinSettings.sharedContext = false;
	osWinSettings.stencilBits = settings->stencilBits;
	osWinSettings.stereo = settings->stereo;
	osWinSettings.visible = true;
	osWinSettings.width = settings->width;
	osWinSettings.x = settings->xPos;
	osWinSettings.y = settings->yPos;

	_window = G3D::GLFWWindow::create(osWinSettings);

	// This is a hack because the G3D GLFWWindow constructor makes the opengl
	// context current. We want it to be current on the renderthread instead of this main thread.
	glfwMakeContextCurrent(NULL);
}

WindowG3D9::~WindowG3D9()
{
	if (_renderDevice != nullptr) {
		_renderDevice->cleanup();
		delete _renderDevice;
	}
}

void WindowG3D9::createRenderDevice()
{
	debugAssertM(_window != nullptr, "Internal window has not be created");
	debugAssertM(_renderDevice == nullptr, "RenderDevice has already been created");

	// creating a renderdevice is not threadsafe since it references G3D::GLCaps.cpp which caches some stuff
	boost::unique_lock<boost::mutex> initRDLock(_RDMUTEX);
	_renderDevice = new G3D::RenderDevice();
	_renderDevice->init(_window);
	initRDLock.unlock();

	_renderDevice->setSwapBuffersAutomatically(false);

	for(int i=0; i < _cameras.size(); i++) {
		dynamic_cast<OffAxisCameraG3D9*>(_cameras[i].get())->setRenderDevice(_renderDevice);
	}
}

void WindowG3D9::pollForInput(std::vector<EventRef> &events)
{
	G3D::GEvent g3dEvent;
	while (_window->pollEvent(g3dEvent)) {
		switch(g3dEvent.type) {
			case G3D::GEventType::QUIT:
				exit(0);
				break;
			case G3D::GEventType::VIDEO_RESIZE:
				events.push_back(EventRef(new Event("WindowResize", glm::vec2(g3dEvent.resize.w, g3dEvent.resize.h))));
				break;
			case G3D::GEventType::KEY_DOWN:
			{
				std::string keyname = getKeyName(g3dEvent.key.keysym.sym);
				std::string mod = getModName(g3dEvent.key.keysym.mod);
				if (mod != "") {
					keyname = keyname+"_"+mod;
				}
				events.push_back(EventRef(new Event("kbd_" + keyname + "_down", getKeyValue(g3dEvent.key.keysym.sym, g3dEvent.key.keysym.mod))));
				break;
			}
			case G3D::GEventType::KEY_REPEAT:
			{
				std::string keyname = getKeyName(g3dEvent.key.keysym.sym);
				std::string mod = getModName(g3dEvent.key.keysym.mod);
				if (mod != "") {
					keyname = keyname+"_"+mod;
				}
				events.push_back(EventRef(new Event("kbd_" + keyname + "_repeat", getKeyValue(g3dEvent.key.keysym.sym, g3dEvent.key.keysym.mod))));
				break;
			}
			case G3D::GEventType::KEY_UP:
			{
				std::string keyname = getKeyName(g3dEvent.key.keysym.sym);
				std::string mod = getModName(g3dEvent.key.keysym.mod);
				if (mod != "") {
					keyname = keyname+"_"+mod;
				}
				events.push_back(EventRef(new Event("kbd_" + keyname + "_up",  getKeyValue(g3dEvent.key.keysym.sym, g3dEvent.key.keysym.mod))));
				break;
			}
			case G3D::GEventType::MOUSE_MOTION:
				_cursorPosition.x = g3dEvent.motion.x;
				_cursorPosition.y = g3dEvent.motion.y;
				events.push_back(EventRef(new Event("mouse_pointer", _cursorPosition)));
				break;
			case G3D::GEventType::MOUSE_BUTTON_DOWN:
				switch (g3dEvent.button.button)
				{
					case 0: //SDL_BUTTON_LEFT:
						events.push_back(EventRef(new Event("mouse_btn_left_down", _cursorPosition)));
						break;
					case 1: //SDL_BUTTON_MIDDLE:
						events.push_back(EventRef(new Event("mouse_btn_middle_down", _cursorPosition)));
						break;
					case 2: //SDL_BUTTON_RIGHT:
						events.push_back(EventRef(new Event("mouse_btn_right_down", _cursorPosition)));
						break;
					default:
						events.push_back(EventRef(new Event("mouse_btn_" + intToString(g3dEvent.button.button) + "_down", _cursorPosition)));
						break;
				}
				break;
			case G3D::GEventType::MOUSE_BUTTON_UP:
				switch (g3dEvent.button.button)
				{
					case 0: //SDL_BUTTON_LEFT:
						events.push_back(EventRef(new Event("mouse_btn_left_up", _cursorPosition)));
						break;
					case 1: //SDL_BUTTON_MIDDLE:
						events.push_back(EventRef(new Event("mouse_btn_middle_up", _cursorPosition)));
						break;
					case 2: //SDL_BUTTON_RIGHT:
						events.push_back(EventRef(new Event("mouse_btn_right_up", _cursorPosition)));
						break;
					default:
						events.push_back(EventRef(new Event("mouse_btn_" + intToString(g3dEvent.button.button) + "_up", _cursorPosition)));
						break;
				}
				break;
			case G3D::GEventType::JOY_AXIS_MOTION:
			case G3D::GEventType::JOY_BALL_MOTION:
			case G3D::GEventType::JOY_HAT_MOTION:
			case G3D::GEventType::JOY_BUTTON_DOWN:
			case G3D::GEventType::JOY_BUTTON_UP:
			case G3D::GEventType::SYSWMEVENT:
			case G3D::GEventType::VIDEO_EXPOSE:
			case G3D::GEventType::GUI_DOWN:
			case G3D::GEventType::GUI_UP:
			case G3D::GEventType::GUI_ACTION: 
			case G3D::GEventType::GUI_CHANGE: 
			case G3D::GEventType::GUI_CANCEL: 
			case G3D::GEventType::GUI_CLOSE:
			case G3D::GEventType::GUI_KEY_FOCUS:
			case G3D::GEventType::FILE_DROP: 
			case G3D::GEventType::MOUSE_BUTTON_CLICK: 
			case G3D::GEventType::FOCUS:
			default:
				std::cout<< "MINVR G3D9 AppKit does not currently support G3D events of type: "<<intToString(g3dEvent.type)<<std::endl;
				std::cout<< "Unsupported Event: "<<g3dEvent.toString()<<std::endl;
				break;
		}
	}
}

std::string WindowG3D9::getKeyName(G3D::GKey::Value key)
{
	switch (key) {
		case G3D::GKey::F1:			return "F1";
		case G3D::GKey::F2:			return "F2";
		case G3D::GKey::F3:			return "F3";
		case G3D::GKey::F4:			return "F4";
		case G3D::GKey::F5:			return "F5";
		case G3D::GKey::F6:			return "F6";
		case G3D::GKey::F7:			return "F7";
		case G3D::GKey::F8:			return "F8";
		case G3D::GKey::F9:			return "F9";
		case G3D::GKey::F10:		return "F10";
		case G3D::GKey::F11:		return "F11";
		case G3D::GKey::F12:		return "F12";
		case G3D::GKey::F13:		return "F13";
		case G3D::GKey::F14:		return "F14";
		case G3D::GKey::F15:		return "F15";
		case G3D::GKey::KP0:		return "KEYPAD0";
		case G3D::GKey::KP1:		return "KEYPAD1";
		case G3D::GKey::KP2:		return "KEYPAD2";
		case G3D::GKey::KP3:		return "KEYPAD3";
		case G3D::GKey::KP4:		return "KEYPAD4";
		case G3D::GKey::KP5:		return "KEYPAD5";
		case G3D::GKey::KP6:		return "KEYPAD6";
		case G3D::GKey::KP7:		return "KEYPAD7";
		case G3D::GKey::KP8:		return "KEYPAD8";
		case G3D::GKey::KP9:		return "KEYPAD9";
		case G3D::GKey::KP_PERIOD:	return "KEYPAD_DECIMAL";
		case G3D::GKey::KP_MULTIPLY:	return "KEYPAD_MULTIPLY";
		case G3D::GKey::KP_MINUS:	return "KEYPAD_MINUS";
		case G3D::GKey::KP_PLUS:	return "KEYPAD_PLUS";
		case G3D::GKey::KP_ENTER:	return "KEYPAD_ENTER";
		case G3D::GKey::KP_EQUALS:	return "KEYPAD_EQUALS";
		case G3D::GKey::LEFT:		return "LEFT";
		case G3D::GKey::RIGHT:		return "RIGHT";
		case G3D::GKey::UP:			return "UP";
		case G3D::GKey::DOWN:		return "DOWN";
		case G3D::GKey::PAGEUP:		return "PAGEUP";
		case G3D::GKey::PAGEDOWN:	return "PAGEDOWN";
		case G3D::GKey::HOME:		return "HOME";
		case G3D::GKey::END:		return "END";
		case G3D::GKey::INSERT:		return "INSERT";
		case G3D::GKey::BACKSPACE:	return "BACKSPACE";
		case G3D::GKey::TAB:		return "TAB";
		case G3D::GKey::RETURN:		return "ENTER";
		case G3D::GKey::ESCAPE:		return "ESC";
		case G3D::GKey::SPACE:		return "SPACE";
		case 48:					return "0";
		case 49:					return "1";
		case 50:					return "2";
		case 51:					return "3";
		case 52:					return "4";
		case 53:					return "5";
		case 54:					return "6";
		case 55:					return "7";
		case 56:					return "8";
		case 57:					return "9";
		case 97:					return "A";
		case 98:					return "B";
		case 99:					return "C";
		case 100:					return "D";
		case 101:					return "E";
		case 102:					return "F";
		case 103:					return "G";
		case 104:					return "H";
		case 105:					return "I";
		case 106:					return "J";
		case 107:					return "K";
		case 108:					return "L";
		case 109:					return "M";
		case 110:					return "N";
		case 111:					return "O";
		case 112:					return "P";
		case 113:					return "Q";
		case 114:					return "R";
		case 115:					return "S";
		case 116:					return "T";
		case 117:					return "U";
		case 118:					return "V";
		case 119:					return "W";
		case 120:					return "X";
		case 121:					return "Y";
		case 122:					return "Z";
		case G3D::GKey::RSHIFT:		return "RIGHT_SHIFT";
		case G3D::GKey::LSHIFT:		return "LEFT_SHIFT";
		case G3D::GKey::RCTRL:		return "RIGHT_CONTROL";
		case G3D::GKey::LCTRL:		return "LEFT_CONTROL";
		case G3D::GKey::RALT:		return "RIGHT_ALT";
		case G3D::GKey::LALT:		return "LEFT_ALT";
		case G3D::GKey::PERIOD:		return "PERIOD";
		case G3D::GKey::COMMA:		return "COMMA";
		case G3D::GKey::LEFTBRACKET: return "LEFT_BRACKET";
		case G3D::GKey::RIGHTBRACKET: return "RIGHT_BRACKET";
		case G3D::GKey::PAUSE:		return "PAUSE";
		case G3D::GKey::MINUS:		return "MINUS";
		case G3D::GKey::SLASH:		return "SLASH";
		case G3D::GKey::SEMICOLON:	return "SEMICOLON";
		case G3D::GKey::EQUALS:		return "EQUALS";
		case G3D::GKey::BACKSLASH:	return "BACKSLASH";
		case G3D::GKey::BACKQUOTE:	return "GRAVE_ACCENT";
		case G3D::GKey::DELETE:		return "DELETE";
		case G3D::GKey::WORLD_1:	return "WORLD1";
		case G3D::GKey::WORLD_2:	return "WORLD2";
		case G3D::GKey::MENU:		return "MENU";
		case G3D::GKey::NUMLOCK:	return "NUM_LOCK";
		case G3D::GKey::CAPSLOCK:	return "CAPS_LOCK";
		case G3D::GKey::SCROLLOCK:	return "SCROLL_LOCK";
		case G3D::GKey::LSUPER:		return "LEFT_SUPER";
		case G3D::GKey::RSUPER:		return "RIGHT_SUPER";
		default:					return "UNKNOWN";
	}
}

std::string WindowG3D9::getModName(G3D::GKeyMod mod)
{
	string modName = "";
	if (mod & G3D::GKeyMod::SHIFT) {
		modName += "SHIFT";
	}
	if (mod & G3D::GKeyMod::CTRL) {
		modName += "CTRL";
	}
	if (mod & G3D::GKeyMod::ALT) {
		modName += "ALT";
	}
	if (mod & G3D::GKeyMod::META) {
		modName += "SUPER";
	}
	return modName;
}

std::string WindowG3D9::getKeyValue(G3D::GKey::Value key, G3D::GKeyMod mod)
{
	string value = "";
	switch (key) {
		case 97:					value  = "a"; break;
		case 98:					value  = "b"; break;
		case 99:					value  = "c"; break;
		case 100:					value  = "d"; break;
		case 101:					value  = "e"; break;
		case 102:					value  = "f"; break;
		case 103:					value  = "g"; break;
		case 104:					value  = "h"; break;
		case 105:					value  = "i"; break;
		case 106:					value  = "j"; break;
		case 107:					value  = "k"; break;
		case 108:					value  = "l"; break;
		case 109:					value  = "m"; break;
		case 110:					value  = "n"; break;
		case 111:					value  = "o"; break;
		case 112:					value  = "p"; break;
		case 113:					value  = "q"; break;
		case 114:					value  = "r"; break;
		case 115:					value  = "s"; break;
		case 116:					value  = "t"; break;
		case 117:					value  = "u"; break;
		case 118:					value  = "v"; break;
		case 119:					value  = "w"; break;
		case 120:					value  = "x"; break;
		case 121:					value  = "y"; break;
		case 122:					value  = "z"; break;
		default:					value = ""; break;
	}

	if (value != "") {
		if (mod & G3D::GKeyMod::SHIFT) {
			boost::to_upper(value);
		}
	}
	else {
		switch(key)
		{
			case G3D::GKey::KP0:		value  = "0"; break;
			case G3D::GKey::KP1:		value  = "1"; break;
			case G3D::GKey::KP2:		value  = "2"; break;
			case G3D::GKey::KP3:		value  = "3"; break;
			case G3D::GKey::KP4:		value  = "4"; break;
			case G3D::GKey::KP5:		value  = "5"; break;
			case G3D::GKey::KP6:		value  = "6"; break;
			case G3D::GKey::KP7:		value  = "7"; break;
			case G3D::GKey::KP8:		value  = "8"; break;
			case G3D::GKey::KP9:		value  = "9"; break;
			case G3D::GKey::KP_PERIOD:	value  = "."; break;
			case G3D::GKey::KP_MULTIPLY:	value  = "*"; break;
			case G3D::GKey::KP_MINUS:	value  = "-"; break;
			case G3D::GKey::KP_PLUS:	value  = "+"; break;
			case G3D::GKey::KP_ENTER:	value  = "\n"; break;
			case G3D::GKey::KP_EQUALS:	value  = "="; break;
			case G3D::GKey::TAB:		value  = "\t"; break;
			case G3D::GKey::RETURN:		value  = "\n"; break;
			case G3D::GKey::SPACE:		value  = " "; break;
			case 48:					value  = "0"; break;
			case 49:					value  = "1"; break;
			case 50:					value  = "2"; break;
			case 51:					value  = "3"; break;
			case 52:					value  = "4"; break;
			case 53:					value  = "5"; break;
			case 54:					value  = "6"; break;
			case 55:					value  = "7"; break;
			case 56:					value  = "8"; break;
			case 57:					value  = "9"; break;
			case G3D::GKey::PERIOD:		value  = "."; break;
			case G3D::GKey::COMMA:		value  = ","; break;
			case G3D::GKey::LEFTBRACKET: value  = "["; break;
			case G3D::GKey::RIGHTBRACKET: value  = "]"; break;
			case G3D::GKey::MINUS:		value  = "-"; break;
			case G3D::GKey::SLASH:		value  = "/"; break;
			case G3D::GKey::SEMICOLON:	value  = ";"; break;
			case G3D::GKey::EQUALS:		value  = "="; break;
			case G3D::GKey::BACKSLASH:	value  = "\\"; break;
			case G3D::GKey::BACKQUOTE:	value  = "`"; break;
			default:					break;
		}
	}
	return value;
}

void WindowG3D9::swapBuffers()
{
	debugAssert(_renderDevice->initialized());
	_renderDevice->swapBuffers();
}

void WindowG3D9::makeContextCurrent()
{
	_window->makeCurrent();	
}

void WindowG3D9::releaseContext()
{
	BOOST_ASSERT_MSG(false, "Release context is not currently supported by the G3D9 app kit");
}

int WindowG3D9::getWidth()
{
	debugAssert(_renderDevice->initialized());
	return _renderDevice->width();
}

int WindowG3D9::getHeight()
{
	debugAssert(_renderDevice->initialized());
	return _renderDevice->height();
}

int WindowG3D9::getXPos()
{
	debugAssert(_renderDevice->initialized());
	return _window->clientRect().x0();
}

int WindowG3D9::getYPos()
{
	debugAssert(_renderDevice->initialized());
	return _window->clientRect().y0();
}

G3D::RenderDevice* WindowG3D9::getRenderDevice()
{
	return _renderDevice;
}

} // end namespace