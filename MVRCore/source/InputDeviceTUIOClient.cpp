//========================================================================
// MinVR
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

#include "MVRCore/InputDeviceTUIOClient.H"

#include "MVRCore/StringUtils.H"
#include "MVRCore/ConfigMap.H"
#include <list>

using namespace G3DLite;

namespace MinVR {

#ifdef USE_TUIO

using namespace TUIO;

InputDeviceTUIOClient::InputDeviceTUIOClient(int port, double xScale, double yScale)
{
	_xScale = xScale;
	_yScale = yScale;
	_tuioClient = new TuioClient(port);
	_tuioClient->connect();

	if (!_tuioClient->isConnected())
	{  std::cerr << "InputDeviceTUIOClient: Cannot connect on port " << port << "." << std::endl;
	}
}



InputDeviceTUIOClient::InputDeviceTUIOClient(const std::string name, Log* log, const ConfigMapRef map)
{
	int  port = map->get( name + "_Port", TUIO_PORT );
	double xs = map->get( name + "_XScaleFactor", 1.0 );
	double ys = map->get( name + "_YScaleFactor", 1.0 );

	_xScale = xs;
	_yScale = ys;
	_tuioClient = new TuioClient(port);
	_tuioClient->connect();

	if (!_tuioClient->isConnected())
	{  std::cout << "InputDeviceTUIOClient: Cannot connect on port " << port << "." << std::endl;
	}
}

InputDeviceTUIOClient::~InputDeviceTUIOClient()
{
	if (_tuioClient) {
		_tuioClient->disconnect();
		delete _tuioClient;
	}
}

void InputDeviceTUIOClient::pollForInput(Array<EventRef> &events)
{
	// Send out events for TUIO "cursors" by polling the TuioClient for the current state  
	std::list<TuioCursor*> cursorList = _tuioClient->getTuioCursors();
	//_tuioClient->lockCursorList();

	// Send "button" up events for cursors that were down last frame, but are now up.
	Array<int> downLast = _cursorsDown.getMembers();
	for (int i=0;i<downLast.size();i++) {
		bool stillDown = false;
		for (std::list<TuioCursor*>::iterator iter = cursorList.begin(); iter!=cursorList.end(); iter++) {
			TuioCursor *tcur = (*iter);
			if (tcur->getCursorID() == downLast[i]) {
				stillDown = true;
			}
		}
		if (!stillDown) {
			events.append(new Event("TUIO_Cursor" + intToString(downLast[i]) + "_up", nullptr, downLast[i]));
			_cursorsDown.remove(downLast[i]);
		}
	}

	// Send "button" down events for cursors that are new and updated positions for all cursors
	for (std::list<TuioCursor*>::iterator iter = cursorList.begin(); iter!=cursorList.end(); iter++) {
		TuioCursor *tcur = (*iter);
		Vector2 pos = Vector2(_xScale*tcur->getX(), _yScale*tcur->getY());

		if (!_cursorsDown.contains(tcur->getCursorID())) {
			events.append(new Event("TUIO_Cursor" + intToString(tcur->getCursorID()) + "_down", pos, nullptr, tcur->getCursorID()));
			_cursorsDown.insert(tcur->getCursorID());
		}

		if (tcur->getMotionSpeed() > 0.0) {
			Vector4 data = Vector4(pos, tcur->getMotionSpeed(), tcur->getMotionAccel());
			events.append(new Event("TUIO_CursorMove" + intToString(tcur->getCursorID()), data, nullptr, tcur->getCursorID()));
		}

		// Can also access several other properties of cursors (speed, acceleration, path followed, etc.)
		//std::cout << "cur " << tcur->getCursorID() << " (" <<  tcur->getSessionID() << ") " << tcur->getX() << " " << tcur->getY() 
		// << " " << tcur->getMotionSpeed() << " " << tcur->getMotionAccel() << " " << std::endl;

		// This is how to access all the points in the path that a cursor follows:     
		//std::list<TuioPoint> path = tuioCursor->getPath();
		//if (path.size() > 0) {
		//  TuioPoint last_point = path.front();
		//  for (std::list<TuioPoint>::iterator point = path.begin(); point!=path.end(); point++) {
		//    last_point.update(point->getX(),point->getY());   
		//  }
		//}
	}
	_tuioClient->unlockCursorList();


	// Unsure what TUIO "objects" are -- perhaps tangible props.  In any case, this is how to access object data:
	std::list<TuioObject*> objectList = _tuioClient->getTuioObjects();
	_tuioClient->lockObjectList();
	for (std::list<TuioObject*>::iterator iter = objectList.begin(); iter!=objectList.end(); iter++) {
		TuioObject* tuioObject = (*iter);    
		int   id    = tuioObject->getSymbolID();
		float xpos  = _xScale*tuioObject->getX();
		float ypos  = _yScale*tuioObject->getY();
		float angle = tuioObject->getAngle()/M_PI*180.0;

		std::string name = "TUIO_Obj" + intToString(id);
		events.append(new Event(name, Vector3(xpos, ypos, angle)));
	}
	_tuioClient->unlockObjectList();
}

#endif //USE_TUIO

};         // end namespace
