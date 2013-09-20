#include "AppKit_G3D9/MVREngineG3D9.H"
#include "G3D9DemoApp.H"
#include "MVRCore/DataFileUtils.H"

int main(int argc, char** argv)
{
	// This is how you add a search path for MinVR to find setup files if they are stored in a
	// directory that differs from the MinVR install directory. Note that the $(G) will be replaced
	// by an environment variable called G.
	MinVR::DataFileUtils::addFileSearchPath("$(G)/src/MinVR/MinVR/MVRCore/vrsetup");
	MinVR::DataFileUtils::addFileSearchPath("$(G)/src/MinVR/MinVR/MVRCore/shaders");

	MinVR::AbstractMVREngine *engine = new MinVR::MVREngineG3D9();
	engine->init(argc, argv);
	MinVR::AbstractMVRAppRef app(new G3D9DemoApp());
	engine->runApp(app);
	delete engine;
}
