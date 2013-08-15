//========================================================================
// MinVR
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

/**
   \author Bret Jackson   
  
   \file  WindowSettings.H
   
   \brief Holds settings for window creation

*/

#ifndef WINDOWSETTINGS_H
#define WINDOWSETTINGS_H

#include <string>
#include <G3DLite/G3DLite.h>

namespace MinVR {

typedef G3DLite::ReferenceCountedPointer<class WindowSettings> WindowSettingsRef;

class WindowSettings : public G3DLite::ReferenceCountedObject
{
public:
	enum StereoType {
		STEREOTYPE_MONO = 0,
		STEREOTYPE_QUADBUFFERED = 1,
		STEREOTYPE_CHECKERBOARD = 2,
		STEREOTYPE_INTERLACEDCOLUMNS = 3,
		STEREOTYPE_INTERLACEDROWS = 4,
		STEREOTYPE_SIDEBYSIDE = 5
	};

	WindowSettings() : width(960), height(600), xPos(0), yPos(0), windowTitle("MinVR"), resizable(true), rgbBits(8),
		alphaBits(8), depthBits(24), stencilBits(8), stereo(false), stereoType(WindowSettings::STEREOTYPE_MONO), msaaSamples(0),
		framed(true), fullScreen(false), visible(true), useGPUAffinity(true), useDebugContext(false) {};
	~WindowSettings() {};

	int width;
	int height;
	int xPos;
	int yPos;
	std::string windowTitle;
	bool resizable;
	int rgbBits;
	int alphaBits;
	int depthBits;
	int stencilBits;
	bool stereo;
	StereoType stereoType;
	int msaaSamples;
	bool framed;
	bool fullScreen;
	bool visible;
	G3DLite::Array<G3DLite::Rect2D> viewports;
	bool useGPUAffinity;
	bool useDebugContext;
};

} // end namespace

#endif