//========================================================================
// MinVR
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

/**
   \author Bret Jackson   
  
   \file  WindowSettings.H
   
   \brief Holds settings for window creation

*/

#ifndef WINDOWSETTINGS_H
#define WINDOWSETTINGS_H

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "MVRCore/Rect2D.H"

namespace MinVR {

typedef std::shared_ptr<class WindowSettings> WindowSettingsRef;

class WindowSettings
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
	std::vector<MinVR::Rect2D> viewports;
	bool useGPUAffinity;
	bool useDebugContext;
};

} // end namespace

#endif