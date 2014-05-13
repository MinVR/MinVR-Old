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

#include "AppKit_G3D9/MVREngineG3D9.H"

namespace MinVR {

MVREngineG3D9::MVREngineG3D9() : AbstractMVREngine()
{
	G3D::initGLG3D();
}

MVREngineG3D9::~MVREngineG3D9()
{
}

void MVREngineG3D9::setupWindowsAndViewports()
{
	glm::dmat4 initialHeadFrame = _configMap->get("InitialHeadFrame", glm::dmat4(1.0));
	
	// InterOcularDistance defaults to 2.5 inches (0.2083 ft). This assumes your coordinate system is in feet.
	double interOcularDistance = _configMap->get("InterOcularDistance", 0.2083);

	// Each window will have its own OpenGL graphics context and run in its own thread.
	// Many applications will only have 1 window, but for fastest performance in a CAVE
	// driven by a multiple GPU computer, you would want to create a separate Window
	// and OpenGL context for each wall, and render each of these in a separate thread.
	int nWindows = _configMap->get("NumWindows", 1);
	for (int w=0;w<nWindows;w++) {
		std::string winStr = "Window" + intToString(w+1) + "_";

		WindowSettingsRef wSettings(new WindowSettings());
		wSettings->width        = _configMap->get(winStr + "Width", wSettings->width);
		wSettings->height       = _configMap->get(winStr + "Height", wSettings->height);
		wSettings->xPos         = _configMap->get(winStr + "X", wSettings->xPos);
		wSettings->yPos         = _configMap->get(winStr + "Y", wSettings->yPos);
		wSettings->fullScreen   = _configMap->get(winStr + "FullScreen", wSettings->fullScreen);
		wSettings->resizable    = _configMap->get(winStr + "Resizable", wSettings->resizable);
		wSettings->framed       = _configMap->get(winStr + "Framed", wSettings->framed);
		wSettings->windowTitle  = _configMap->get(winStr + "Caption", wSettings->windowTitle);
		wSettings->useDebugContext = _configMap->get(winStr + "UseDebugContext", wSettings->useDebugContext);
		wSettings->msaaSamples  = _configMap->get(winStr + "MSAASamples", wSettings->msaaSamples);
		wSettings->rgbBits      = _configMap->get(winStr + "RGBBits", wSettings->rgbBits);
		wSettings->depthBits    = _configMap->get(winStr + "DepthBits", wSettings->depthBits);
		wSettings->stencilBits  = _configMap->get(winStr + "StencilBits", wSettings->stencilBits);
		wSettings->alphaBits    = _configMap->get(winStr + "AlphaBits", wSettings->alphaBits);
		wSettings->visible      = _configMap->get(winStr + "Visible", wSettings->visible);
		wSettings->useGPUAffinity = _configMap->get(winStr + "UseGPUAffinity", wSettings->useGPUAffinity);
		wSettings->stereo		= _configMap->get(winStr + "Stereo", wSettings->stereo);

		//wSettings.mouseVisible = _configMap->get(winStr + "MouseVisible", wSettings.mouseVisible);

		std::string stereoStr = _configMap->get(winStr + "StereoType", "Mono");
		if (stereoStr == "Mono") {
			wSettings->stereoType = WindowSettings::STEREOTYPE_MONO;
		}
		else if (stereoStr == "QuadBuffered") {
			wSettings->stereoType = WindowSettings::STEREOTYPE_QUADBUFFERED;
		}
		else if (stereoStr == "Checkerboard") {
			wSettings->stereoType = WindowSettings::STEREOTYPE_CHECKERBOARD;
		}
		else if (stereoStr == "InterlacedColumns") {
			wSettings->stereoType = WindowSettings::STEREOTYPE_INTERLACEDCOLUMNS;
		}
		else if (stereoStr == "InterlacedRows") {
			wSettings->stereoType = WindowSettings::STEREOTYPE_INTERLACEDROWS;
		}
		else if (stereoStr == "SideBySide") {
			wSettings->stereoType = WindowSettings::STEREOTYPE_SIDEBYSIDE;
		}
		else {
			std::stringstream ss;
			ss << "Fatal error: Unrecognized value for " + winStr + "StereoType: " + stereoStr;
			BOOST_ASSERT_MSG(false, ss.str().c_str());
		}


		// Within each window, you can have multiple viewports.  Each viewport will render
		// with a separate projection matrix, so this is used for drawing left and right eyes
		// in a side-by-side layout or for rendering different walls of a CAVE in the situation
		// where you stretch a large window across a virtual desktop that covers all the walls
		// of the cave and render using one thread rather than using a separate window and 
		// rendering thread for each wall.
		int nViewports = _configMap->get(winStr + "NumViewports", 1);
		std::vector<AbstractCameraRef> cameras;
		for (int v=0;v<nViewports;v++) {
			std::string viewportStr = winStr + "Viewport" + intToString(v+1) + "_";
			
			int width    = _configMap->get(viewportStr + "Width", wSettings->width);
			int height   = _configMap->get(viewportStr + "Height", wSettings->height);
			int x        = _configMap->get(viewportStr + "X", 0);
			int y        = _configMap->get(viewportStr + "Y", 0);
			wSettings->viewports.push_back(MinVR::Rect2D::xywh(x,y,width,height));

			std::string cameraStr = _configMap->get(viewportStr + "CameraType", "OffAxis");
			if (cameraStr == "OffAxis") {
				glm::dvec3 topLeft  = _configMap->get(viewportStr + "TopLeft", glm::dvec3(-1.0, 1.0, 0.0));
				glm::dvec3 topRight = _configMap->get(viewportStr + "TopRight", glm::dvec3(1.0, 1.0, 0.0));
				glm::dvec3 botLeft  = _configMap->get(viewportStr + "BotLeft", glm::dvec3(-1.0, -1.0, 0.0));
				glm::dvec3 botRight = _configMap->get(viewportStr + "BotRight", glm::dvec3(1.0, -1.0, 0.0));
				double nearClip = _configMap->get(viewportStr + "NearClip", 0.01);
				double farClip  = _configMap->get(viewportStr + "FarClip", 1000.0);
				AbstractCameraRef cam(new OffAxisCameraG3D9(topLeft, topRight, botLeft, botRight, initialHeadFrame, interOcularDistance, nearClip, farClip));
				cameras.push_back(cam);
			}
			else if (cameraStr == "Traditional") {
				// TODO: Implement this for use with systems like desktop haptics.
				BOOST_ASSERT_MSG(false, "Traditional camera not yet implemented");
			}
			else {
				std::stringstream ss;
				ss << "Fatal error: Unrecognized value for " << viewportStr << "CameraType: " << cameraStr;
				BOOST_ASSERT_MSG(false, ss.str().c_str());
			}
		}

		WindowRef window = createWindow(wSettings, cameras);
		_windows.push_back(window);
	}

	for (int i=0;i<_windows.size();i++) {
		_windows[i]->updateHeadTrackingForAllViewports(initialHeadFrame);
	}
}

void MVREngineG3D9::initializeContextSpecificVars(int threadId, WindowRef window)
{
	// We create the render device here because it sets some context specific things when created
	WindowG3D9* win = dynamic_cast<WindowG3D9*>(window.get());
	win->createRenderDevice();
}

WindowRef MVREngineG3D9::createWindow(WindowSettingsRef settings, std::vector<AbstractCameraRef> cameras)
{
	WindowRef window(new WindowG3D9(settings, cameras));
	return window;
}

} // end namespace