#include "HapticApp.H"
#include "HapticAccess.H"
#include "GL/glut.h"

HapticApp* HapticApp::_happ = NULL;
void HapticApp_drawWindow();

void HapticApp::run() {
	if (!_running) {
		_running = true;
		glutMainLoop();
	}
}

HapticApp::HapticApp(int width, int height) {
	initGlut(width, height);
	initHL();
	_happ = this;
	_running = false;
}

void HapticApp_drawWindow() {
	HapticApp::_happ->tick();
	HapticApp::_happ->draw(true);
	HapticApp::_happ->draw(false);
	glutSwapBuffers();
	glutPostRedisplay();
}

HapticApp::~HapticApp() {
}


void HapticApp::tick() {
}

void HapticApp::draw(bool haptic) {
}

void HapticApp::initGlut(int width, int height) {
	_windowWidth = width;
	_windowHeight = height;
	char **fakeArgv = new char*();
	fakeArgv[0] = "C:\\";
	int fakeArgc = 1;
	glutInit(&fakeArgc, fakeArgv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_STENCIL);
	glutCreateWindow("OpenHaptics");
	glutReshapeWindow(_windowWidth, _windowHeight);
	glutDisplayFunc(HapticApp_drawWindow);
}