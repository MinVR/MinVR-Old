//========================================================================
// MinVR - AppKit G3D9
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
	glm::dvec2 _cursorPosition;

	std::string getKeyName(G3D::GKey::Value key);
	std::string getModName(G3D::GKeyMod mod);
	std::string getKeyValue(G3D::GKey::Value key, G3D::GKeyMod mod);

	static boost::mutex _RDMUTEX;	
};

} // end namespace

#endif