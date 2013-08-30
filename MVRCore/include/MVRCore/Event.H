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

#ifndef MINVREVENT_H
#define MINVREVENT_H

#include <glm/glm.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "MVRCore/StringUtils.H"

namespace MinVR {

class AbstractWindow;
typedef boost::shared_ptr<class AbstractWindow> WindowRef;

class Event;
typedef boost::shared_ptr<class Event> EventRef;

/// Creates a copy of the Event pointed to by e and returns a ref
/// counted pointer to the new copy.
EventRef createCopyOfEvent(EventRef e);

/** G3DVR Event class.  To keep things simple, there are no subclasses
of Event.  The type of data that the event carries is interpreted
differently based on the value of the type of the event.  Button
Events are typically sent by devices as two separate
EVENTTYPE_STANDARD Events, the first named ButtonName_down and
then when the button is released ButtonName_up.
*/
class Event
{
public:

	enum EventType {  
		EVENTTYPE_STANDARD = 0,        /// standard type, carries no additional info
		EVENTTYPE_1D = 1,              /// event that stores 1D data in a double
		EVENTTYPE_2D = 2,              /// stores two doubles
		EVENTTYPE_3D = 3,              /// stores three doubles
		EVENTTYPE_4D = 4,			   /// stores four doubles
		EVENTTYPE_COORDINATEFRAME = 5, /// stores a CoordinateFrame
		EVENTTYPE_MSG = 6              /// stores a std::string
	};

	Event(const std::string &name, const WindowRef window = nullptr, const int id = -1);
	Event(const std::string &name, const double data, const WindowRef window = nullptr, const int id = -1);
	Event(const std::string &name, const glm::vec2 &data, const WindowRef window = nullptr, const int id = -1);
	Event(const std::string &name, const glm::vec3 &data, const WindowRef window = nullptr, const int id = -1);
	Event(const std::string &name, const glm::vec4 &data, const WindowRef window = nullptr, const int id = -1);
	Event(const std::string &name, const glm::mat4 &data, const WindowRef window = nullptr, const int id = -1);
	Event(const std::string &name, const std::string &data, const WindowRef window = nullptr, const int id = -1);
	virtual ~Event();
	
	std::string getName() const;
	EventType getType() const;
	int getId() const;
	WindowRef getWindow() const;

	double get1DData();
	glm::vec2 get2DData();
	glm::vec3 get3DData();
	glm::vec4 get4DData();
	glm::mat4 getCoordinateFrameData();
	std::string getMsgData();
	boost::posix_time::ptime getTimestamp();

	bool operator<(Event other) const;
	bool operator<(EventRef otherRef) const;
    
	std::string toString();

	void rename(const std::string &newname);

protected:
	std::string _name;
	int	_id;
	WindowRef _window;
	boost::posix_time::ptime _timestamp;
	EventType _type;
	double _data1D;
	glm::vec2 _data2D;
	glm::vec3 _data3D;
	glm::vec4 _data4D;
	glm::mat4 _dataCF;
	std::string _dataMsg;
};


} // end namespace

#endif

