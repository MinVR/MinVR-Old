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
	glm::mat4 initialHeadFrame = _configMap->get("InitialHeadFrame", glm::mat4(1.0));
	
	// InterOcularDistance defaults to 2.5 inches (0.2083 ft). This assumes your coordinate system is in feet.
	float interOcularDistance = _configMap->get("InterOcularDistance", 0.2083);

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
			int x        = _configMap->get(viewportStr + "X", 0.0);
			int y        = _configMap->get(viewportStr + "Y", 0.0);
			wSettings->viewports.push_back(MinVR::Rect2D::xywh(x,y,width,height));

			std::string cameraStr = _configMap->get(viewportStr + "CameraType", "OffAxis");
			if (cameraStr == "OffAxis") {
				glm::vec3 topLeft  = _configMap->get(viewportStr + "TopLeft", glm::vec3(-1.0, 1.0, 0.0));
				glm::vec3 topRight = _configMap->get(viewportStr + "TopRight", glm::vec3(1.0, 1.0, 0.0));
				glm::vec3 botLeft  = _configMap->get(viewportStr + "BotLeft", glm::vec3(-1.0, -1.0, 0.0));
				glm::vec3 botRight = _configMap->get(viewportStr + "BotRight", glm::vec3(1.0, -1.0, 0.0));
				float nearClip = _configMap->get(viewportStr + "NearClip", 0.01);
				float farClip  = _configMap->get(viewportStr + "FarClip", 1000.0);
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