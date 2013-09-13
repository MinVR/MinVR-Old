#include "G3D9DemoApp.H"

using namespace G3D;
using namespace MinVR;

G3D9DemoApp::G3D9DemoApp()
{
}

G3D9DemoApp::~G3D9DemoApp()
{
}

void G3D9DemoApp::doUserInputAndPreDrawComputation(const std::vector<MinVR::EventRef> &events, double synchronizedTime)
{
	for(int i=0; i < events.size(); i++) {
		if (events[i]->getName() == "kbd_ESC_down") {
			exit(0);
		}
	}
}

void G3D9DemoApp::initializeContextSpecificVars(int threadId, MinVR::WindowRef window)
{
	initVBO();

	RenderDevice* rd = dynamic_cast<WindowG3D9*>(window.get())->getRenderDevice();
	rd->setColorClearValue(Color4(0.2, 0.2, 0.2, 1.0));

}

void G3D9DemoApp::initVBO()
{
	// Allocate on CPU
	Array<Vector3> cpuVertex;
	Array<Vector3> cpuNormals;
	Array<Color3> cpuColors;
	Array<int> cpuIndex;

	cpuVertex.append(Vector3(1, 1, 1));
	cpuVertex.append(Vector3(-1, 1, 1));
	cpuVertex.append(Vector3(-1,-1, 1));
	cpuVertex.append(Vector3(-1,-1, 1));
	cpuVertex.append(Vector3(1,-1, 1));
	cpuVertex.append(Vector3(1, 1, 1));
	cpuVertex.append(Vector3(1, 1, 1));
	cpuVertex.append(Vector3(1,-1, 1));
	cpuVertex.append(Vector3(1,-1,-1));
	cpuVertex.append(Vector3(1,-1,-1));
	cpuVertex.append(Vector3(1, 1,-1));
	cpuVertex.append(Vector3(1, 1, 1));
	cpuVertex.append(Vector3(1, 1, 1));
	cpuVertex.append(Vector3(1, 1,-1));
	cpuVertex.append(Vector3(-1, 1,-1));
	cpuVertex.append(Vector3(-1, 1,-1));
	cpuVertex.append(Vector3(-1, 1, 1));
	cpuVertex.append(Vector3(1, 1, 1));
	cpuVertex.append(Vector3(-1, 1, 1));
	cpuVertex.append(Vector3(-1, 1,-1));
	cpuVertex.append(Vector3(-1,-1,-1));
	cpuVertex.append(Vector3(-1,-1,-1));
	cpuVertex.append(Vector3(-1,-1, 1));
	cpuVertex.append(Vector3(-1, 1, 1));
	cpuVertex.append(Vector3(-1,-1,-1));
	cpuVertex.append(Vector3(1,-1,-1));
	cpuVertex.append(Vector3(1,-1, 1));
	cpuVertex.append(Vector3(1,-1, 1));
	cpuVertex.append(Vector3(-1,-1, 1));
	cpuVertex.append(Vector3(-1,-1,-1));
	cpuVertex.append(Vector3(1,-1,-1));
	cpuVertex.append(Vector3(-1,-1,-1));
	cpuVertex.append(Vector3(-1, 1,-1));
	cpuVertex.append(Vector3(-1, 1,-1));
	cpuVertex.append(Vector3(1, 1,-1));
	cpuVertex.append(Vector3(1,-1,-1));

	// normal array
	cpuNormals.append(Vector3(0, 0, 1));
	cpuNormals.append(Vector3(0, 0, 1));
	cpuNormals.append(Vector3(0, 0, 1));
	cpuNormals.append(Vector3(0, 0, 1));
	cpuNormals.append(Vector3(0, 0, 1));
	cpuNormals.append(Vector3(0, 0, 1));
	cpuNormals.append(Vector3(1, 0, 0));
	cpuNormals.append(Vector3(1, 0, 0));
	cpuNormals.append(Vector3(1, 0, 0));
	cpuNormals.append(Vector3(1, 0, 0));
	cpuNormals.append(Vector3(1, 0, 0));
	cpuNormals.append(Vector3(1, 0, 0));
	cpuNormals.append(Vector3(0, 1, 0));
	cpuNormals.append(Vector3(0, 1, 0));
	cpuNormals.append(Vector3(0, 1, 0));
	cpuNormals.append(Vector3(0, 1, 0));
	cpuNormals.append(Vector3(0, 1, 0));
	cpuNormals.append(Vector3(0, 1, 0));
	cpuNormals.append(Vector3(-1, 0, 0));
	cpuNormals.append(Vector3(-1, 0, 0));
	cpuNormals.append(Vector3(-1, 0, 0));
	cpuNormals.append(Vector3(-1, 0, 0));
	cpuNormals.append(Vector3(-1, 0, 0));
	cpuNormals.append(Vector3(-1, 0, 0));
	cpuNormals.append(Vector3(0,-1, 0));
	cpuNormals.append(Vector3(0,-1, 0));
	cpuNormals.append(Vector3(0,-1, 0));
	cpuNormals.append(Vector3(0,-1, 0));
	cpuNormals.append(Vector3(0,-1, 0));
	cpuNormals.append(Vector3(0,-1, 0));
	cpuNormals.append(Vector3(0, 0,-1));
	cpuNormals.append(Vector3(0, 0,-1));
	cpuNormals.append(Vector3(0, 0,-1));
	cpuNormals.append(Vector3(0, 0,-1));
	cpuNormals.append(Vector3(0, 0,-1));
	cpuNormals.append(Vector3(0, 0,-1));

	// color array
	cpuColors.append(Color3(1, 1, 1));
	cpuColors.append(Color3(1, 1, 0));
	cpuColors.append(Color3(1, 0, 0));
	cpuColors.append(Color3(1, 0, 0));
	cpuColors.append(Color3(1, 0, 1));
	cpuColors.append(Color3(1, 1, 1));
	cpuColors.append(Color3(1, 1, 1));
	cpuColors.append(Color3(1, 0, 1));
	cpuColors.append(Color3(0, 0, 1));
	cpuColors.append(Color3(0, 0, 1));
	cpuColors.append(Color3(0, 1, 1));
	cpuColors.append(Color3(1, 1, 1));
	cpuColors.append(Color3(1, 1, 1));
	cpuColors.append(Color3(0, 1, 1));
	cpuColors.append(Color3(0, 1, 0));
	cpuColors.append(Color3(0, 1, 0));
	cpuColors.append(Color3(1, 1, 0));
	cpuColors.append(Color3(1, 1, 1));
	cpuColors.append(Color3(1, 1, 0));
	cpuColors.append(Color3(0, 1, 0));
	cpuColors.append(Color3(0, 0, 0));
	cpuColors.append(Color3(0, 0, 0));
	cpuColors.append(Color3(1, 0, 0));
	cpuColors.append(Color3(1, 1, 0));
	cpuColors.append(Color3(0, 0, 0));
	cpuColors.append(Color3(0, 0, 1));
	cpuColors.append(Color3(1, 0, 1));
	cpuColors.append(Color3(1, 0, 1));
	cpuColors.append(Color3(1, 0, 0));
	cpuColors.append(Color3(0, 0, 0));
	cpuColors.append(Color3(0, 0, 1));
	cpuColors.append(Color3(0, 0, 0));
	cpuColors.append(Color3(0, 1, 0));
	cpuColors.append(Color3(0, 1, 0));
	cpuColors.append(Color3(0, 1, 1));
	cpuColors.append(Color3(0, 0, 1));

	for(int i=0; i < cpuVertex.size(); i++) {
		cpuIndex.append(i);
	}


	// Upload to GPU
	shared_ptr<VertexBuffer> vbuf = VertexBuffer::create(((sizeof(Vector3)*2) * cpuVertex.size()) + (sizeof(Color3)*cpuColors.size()) + (sizeof(int) * cpuIndex.size()));
	//_vbuffer.reset(vbuf.get());
	_gpuVertex.reset(new AttributeArray(cpuVertex, vbuf));
	_gpuNormals.reset(new AttributeArray(cpuNormals, vbuf));
	_gpuColors.reset(new AttributeArray(cpuColors, vbuf));
	_gpuIndex.reset(new IndexStream(cpuIndex, vbuf));
}

void G3D9DemoApp::drawGraphics(int threadId, MinVR::AbstractCameraRef camera, MinVR::WindowRef window)
{
	G3D::RenderDevice* rd = dynamic_cast<WindowG3D9*>(window.get())->getRenderDevice();

	rd->beginFrame();
	rd->pushState();

	glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
	glm::vec2 rotAngles(45.0, 45.0);
	glm::mat4 rotate1 = glm::rotate(translate, rotAngles.y, glm::vec3(0.0,1.0,0.0));
	camera->setObjectToWorldMatrix(glm::rotate(rotate1, rotAngles.x, glm::vec3(1.0,0,0)));

	rd->setShadeMode(RenderDevice::SHADE_SMOOTH);

	rd->beginIndexedPrimitives();
	rd->setVertexArray(*(_gpuVertex.get()));
	rd->setNormalArray(*(_gpuNormals.get()));
	rd->setColorArray(*(_gpuColors.get()));
	rd->sendIndices(PrimitiveType::TRIANGLES, *(_gpuIndex.get()));
	rd->endIndexedPrimitives();
		
	
	/*
	rd->beginPrimitive(G3D::PrimitiveType::TRIANGLES);
	rd->setColor(G3D::Color3(1.f, 0.f, 0.f));
	rd->setNormal(Vector3(0,0,1));
	rd->sendVertex(Vector3(-0.3f, -0.2f, -1.f));
	rd->setColor(G3D::Color3(0.f, 1.0f, 0.f));
	rd->sendVertex(Vector3(0.3f, -0.2f, -1.0f));
	rd->setColor(G3D::Color3(0.f, 0.f, 1.f));
	rd->sendVertex(Vector3(0.f, 0.3f, -1.f));
	rd->endPrimitive();
	*/

	rd->popState();
	rd->endFrame();
}