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

#ifndef WINDOWG3D9_H
#define WINDOWG3D9_H

#include "MVRCore/AbstractWindow.H"
#include "MVRCore/Event.H"
#include "AppKit_G3D9/OffAxisCameraG3D9.H"
#include <GLG3D/RenderDevice.h>
#include <GLG3D/GKey.h>
#include <GLG3D/GEvent.h>
#include <GLG3D/GLFWWindow.h>
#include <boost/thread.hpp>

namespace MinVR {

class WindowG3D9 : public AbstractWindow
{
public:
	WindowG3D9(WindowSettingsRef settings, std::vector<AbstractCameraRef> cameras);
	~WindowG3D9();

	void pollForInput(std::vector<EventRef> &events);
	void swapBuffers();
	void makeContextCurrent();
	void releaseContext();
	int getWidth();
	int getHeight();
	int getXPos();
	int getYPos();
	void createRenderDevice();
	G3D::RenderDevice* getRenderDevice(); 
	
private:
	G3D::RenderDevice* _renderDevice;
	G3D::GLFWWindow* _window;
	glm::vec2 _cursorPosition;

	std::string getKeyName(G3D::GKey::Value key);
	std::string getModName(G3D::GKeyMod mod);
	std::string getKeyValue(G3D::GKey::Value key, G3D::GKeyMod mod);

	static boost::mutex _RDMUTEX;	
};

} // end namespace

#endif