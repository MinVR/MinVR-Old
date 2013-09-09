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

namespace MinVR {

using namespace std;

WindowG3D9::WindowG3D9(WindowSettingsRef settings, std::vector<AbstractCameraRef> cameras) : AbstractWindow(settings, cameras)
{
	_renderDevice = nullptr;
}

WindowG3D9::~WindowG3D9()
{
}

void WindowG3D9::createRenderDevice()
{
	debugAssertM(_renderDevice == nullptr, "RenderDevice has already been created");
	_renderDevice.reset(new G3D::RenderDevice());
	_renderDevice->init(_window);
	_renderDevice->setSwapBuffersAutomatically(false);

	for(int i=0; i < _cameras.size(); i++) {
		dynamic_cast<OffAxisCameraG3D9*>(_cameras[i].get())->setRenderDevice(_renderDevice);
	}
}

void WindowG3D9::pollForInput(std::vector<EventRef> &events)
{
	
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

shared_ptr<G3D::RenderDevice> WindowG3D9::getRenderDevice()
{
	return _renderDevice;
}

} // end namespace