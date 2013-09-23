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

#ifndef MVRENGINEGLFW_H
#define MVRENGINEGLFW_H

#include "AppKit_GLFW/WindowGLFW.H"
#include "MVRCore/AbstractMVREngine.H"
#include <GLFW/glfw3.h>

namespace MinVR {

/*! @brief VR Engine for raw OpenGL apps based on GLFW
 *
 *  This engine is used for raw OpenGL apps. It depends on the GLFW windowing libarary
 *  to open windows, and collect keyboard and mouse input.
 */
class MVREngineGLFW : public AbstractMVREngine
{
public:
	MVREngineGLFW();
	~MVREngineGLFW();

	/*! @brief Runs a VR application
	 *
	 *  Runs the passed in VR application. This override will terminate the application
	 *  if one of the GLFW windows is closed.
	 */
	void runApp(AbstractMVRAppRef app) override;

	/*! @brief Creates  GLFW window
	*/
	WindowRef createWindow(WindowSettingsRef settings, std::vector<AbstractCameraRef> cameras);

	/*! @brief GLFW error callback
	 *
	 *  Callback method to throw an assertion if GLFW has an error.
	 */
	static void error_callback(int error, const char* description);
};

} // end namespace

#endif