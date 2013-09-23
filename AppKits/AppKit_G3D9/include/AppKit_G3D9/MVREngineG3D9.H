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

#ifndef MVRENGINEG3D9_H
#define MVRENGINEG3D9_H

#include "AppKit_G3D9/WindowG3D9.H"
#include "MVRCore/AbstractMVREngine.H"
#include "AppKit_G3D9/OffAxisCameraG3D9.H"
#include <G3D/G3D.h>
#include <GLG3D/GLG3D.h>

namespace MinVR {

/*! @brief VR Engine for raw OpenGL apps based on GLFW
 *
 *  This engine is used for raw OpenGL apps. It depends on the GLFW windowing libarary
 *  to open windows, and collect keyboard and mouse input.
 */
class MVREngineG3D9 : public AbstractMVREngine
{
public:
	MVREngineG3D9();
	~MVREngineG3D9();

	/*! @brief Initialize thread specific variables
	 *
	 * Called from each renderthread to initialize needed variables
	 *
	 * @param[in] A thread specific id
	 * @param[in] Window reference for the calling renderthread.
	 */
	virtual void initializeContextSpecificVars(int threadId, WindowRef window);

	/*! @brief Creates  G3D9 window
	*/
	virtual WindowRef createWindow(WindowSettingsRef settings, std::vector<AbstractCameraRef> cameras);

protected:

	/*! @brief Creates windows and viewports
	 *
	 *  Called from init to create windows and viewports based on the vrsetup file
	 *  This is an override to create G3D9 specific cameras.
	 *  
	 *  @sa init
	 */
	virtual void setupWindowsAndViewports();
};

} // end namespace

#endif