#ifndef G3D9DEMOAPP_H
#define G3D9DEMOAPP_H

#include "MVRCore/AbstractMVRApp.H"
#include "MVRCore/AbstractCamera.H"
#include "MVRCore/AbstractWindow.H"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "MVRCore/Event.H"
#include <boost/thread.hpp>
#include <vector>
#include "AppKit_G3D9/WindowG3D9.H"
#include <G3D/G3D.h>
#include <GLG3D/GLG3D.h>

class G3D9DemoApp : public MinVR::AbstractMVRApp
{
public:
	G3D9DemoApp();
	~G3D9DemoApp();

	void doUserInputAndPreDrawComputation(const std::vector<MinVR::EventRef> &events, double synchronizedTime);
	void initializeContextSpecificVars(int threadId, MinVR::WindowRef window);
	void drawGraphics(int threadId, MinVR::AbstractCameraRef camera, MinVR::WindowRef window);

private:
	void initVBO();
	boost::thread_specific_ptr<G3D::AttributeArray> _gpuVertex;
	boost::thread_specific_ptr<G3D::AttributeArray> _gpuNormals;
	boost::thread_specific_ptr<G3D::AttributeArray> _gpuColors;
	boost::thread_specific_ptr<G3D::IndexStream> _gpuIndex;

	//boost::thread_specific_ptr<G3D::VertexBuffer> _vbuffer;
	
};

#endif