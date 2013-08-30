#include "AppKit_GLFW/MVREngineGLFW.H"
#include "GLFWDemoApp.H"
#include "MVRCore/DataFileUtils.H"

int main(int argc, char** argv)
{
	// This is how you add a search path for MinVR to find setup files if they are stored in a
	// directory that differs from the MinVR install directory. Note that the $(G) will be replaced
	// by an environment variable called G.
	MinVR::DataFileUtils::addFileSearchPath("$(G)/src/MinVR/MVRCore/vrsetup");
	MinVR::DataFileUtils::addFileSearchPath("$(G)/src/MinVR/MVRCore/shaders");

	MinVR::AbstractMVREngine *engine = new MinVR::MVREngineGLFW();
	engine->init(argc, argv);
	MinVR::AbstractMVRAppRef app(new GLFWDemoApp());
	engine->runApp(app);
	delete engine;
}

