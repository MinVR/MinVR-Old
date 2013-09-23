//========================================================================
// MinVR
// Platform:    Any
// API version: 1.0
//------------------------------------------------------------------------
// Copyright (c) 2013 Regents of the University of Minnesota and Brown University
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
// * Neither the name of the University of Minnesota, Brown University, nor the names of its
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

#include "MVRCore/InputDeviceTUIOClient.H"

#include "MVRCore/StringUtils.H"
#include "MVRCore/ConfigMap.H"
#include <list>

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
	{  
		std::cout << "InputDeviceTUIOClient: Cannot connect on port " << port << "." << std::endl;
	}
}



InputDeviceTUIOClient::InputDeviceTUIOClient(const std::string name, const ConfigMapRef map)
{
	int  port = map->get( name + "_Port", TUIO_PORT );
	double xs = map->get( name + "_XScaleFactor", 1.0 );
	double ys = map->get( name + "_YScaleFactor", 1.0 );

	_xScale = xs;
	_yScale = ys;
	_tuioClient = new TuioClient(port);
	_tuioClient->connect();

	if (!_tuioClient->isConnected())
	{ 
		std::cout << "InputDeviceTUIOClient: Cannot connect on port " << port << "." << std::endl;
	}
}

InputDeviceTUIOClient::~InputDeviceTUIOClient()
{
	if (_tuioClient) {
		_tuioClient->disconnect();
		delete _tuioClient;
	}
}

void InputDeviceTUIOClient::pollForInput(std::vector<EventRef> &events)
{
	// Send out events for TUIO "cursors" by polling the TuioClient for the current state  
	std::list<TuioCursor*> cursorList = _tuioClient->getTuioCursors();
	//_tuioClient->lockCursorList();

	// Send "button" up events for cursors that were down last frame, but are now up.
	for ( auto downLast_it = _cursorsDown.begin(); downLast_it!= _cursorsDown.end(); ++downLast_it ) {
		bool stillDown = false;
		for (std::list<TuioCursor*>::iterator iter = cursorList.begin(); iter!=cursorList.end(); iter++) {
			TuioCursor *tcur = (*iter);
			if (tcur->getCursorID() == *downLast_it) {
				stillDown = true;
			}
		}
		if (!stillDown) {
			events.push_back(EventRef(new Event("TUIO_Cursor" + intToString(*downLast_it) + "_up", nullptr, *downLast_it)));
			_cursorsDown.erase (downLast_it);
		}
	}

	// Send "button" down events for cursors that are new and updated positions for all cursors
	for (std::list<TuioCursor*>::iterator iter = cursorList.begin(); iter!=cursorList.end(); iter++) {
		TuioCursor *tcur = (*iter);
		glm::vec2 pos = glm::vec2(_xScale*tcur->getX(), _yScale*tcur->getY());

		if (_cursorsDown.find(tcur->getCursorID()) != _cursorsDown.end()) {
			events.push_back(EventRef(new Event("TUIO_Cursor" + intToString(tcur->getCursorID()) + "_down", pos, nullptr, tcur->getCursorID())));
			_cursorsDown.insert(tcur->getCursorID());
		}

		if (tcur->getMotionSpeed() > 0.0) {
			glm::vec4 data = glm::vec4(pos, tcur->getMotionSpeed(), tcur->getMotionAccel());
			events.push_back(EventRef(new Event("TUIO_CursorMove" + intToString(tcur->getCursorID()), data, nullptr, tcur->getCursorID())));
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
		events.push_back(EventRef(new Event(name, glm::vec3(xpos, ypos, angle))));
	}
	_tuioClient->unlockObjectList();
}

#endif //USE_TUIO

};         // end namespace
