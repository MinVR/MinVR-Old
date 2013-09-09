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
}

void G3D9DemoApp::initializeContextSpecificVars(int threadId, MinVR::WindowRef window)
{
	initVBO();
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

	// Upload to GPU
	shared_ptr<VertexBuffer> vbuf = VertexBuffer::create((sizeof(Vector3)*3) * cpuVertex.size() + sizeof(int) * cpuIndex.size());
	_vbuffer.reset(vbuf.get());
	_gpuVertex.reset(new AttributeArray(cpuVertex, vbuf));
	_gpuNormals.reset(new AttributeArray(cpuNormals, vbuf));
	_gpuColors.reset(new AttributeArray(cpuColors, vbuf));
	_gpuIndex.reset(new IndexStream(cpuIndex, vbuf));
}

void G3D9DemoApp::drawGraphics(int threadId, MinVR::AbstractCameraRef camera, MinVR::WindowRef window)
{
	shared_ptr<RenderDevice> rd = dynamic_cast<WindowG3D9*>(window.get())->getRenderDevice();

	rd->pushState();

	rd->beginIndexedPrimitives();
	rd->setVertexArray(*(_gpuVertex.get()));
	rd->setNormalArray(*(_gpuNormals.get()));
	rd->setColorArray(*(_gpuColors.get()));
	rd->sendIndices(PrimitiveType::TRIANGLES, *(_gpuIndex.get()));
	rd->endIndexedPrimitives();

	rd->popState();
}