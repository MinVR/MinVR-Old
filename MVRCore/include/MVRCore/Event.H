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

#include <G3DLite/G3DLite.h>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace MinVR {

class AbstractWindow;
typedef G3DLite::ReferenceCountedPointer<class AbstractWindow> WindowRef;

class Event;
typedef G3DLite::ReferenceCountedPointer<class Event> EventRef;

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
class Event : public G3DLite::ReferenceCountedObject
{
public:

	enum EventType {  
		EVENTTYPE_STANDARD = 0,        /// standard type, carries no additional info
		EVENTTYPE_1D = 1,              /// event that stores 1D data in a double
		EVENTTYPE_2D = 2,              /// stores two doubles
		EVENTTYPE_3D = 3,              /// stores three doubles
		EVENTTYPE_4D = 4,			   /// stores four doubles
		EVENTTYPE_COORDINATEFRAME = 5, /// stores a G3DLite::CoordinateFrame
		EVENTTYPE_MSG = 6              /// stores a std::string
	};

	Event(const std::string &name, const WindowRef window = nullptr, const int id = -1);
	Event(const std::string &name, const double data, const WindowRef window = nullptr, const int id = -1);
	Event(const std::string &name, const G3DLite::Vector2 &data, const WindowRef window = nullptr, const int id = -1);
	Event(const std::string &name, const G3DLite::Vector3 &data, const WindowRef window = nullptr, const int id = -1);
	Event(const std::string &name, const G3DLite::Vector4 &data, const WindowRef window = nullptr, const int id = -1);
	Event(const std::string &name, const G3DLite::CoordinateFrame &data, const WindowRef window = nullptr, const int id = -1);
	Event(const std::string &name, const std::string &data, const WindowRef window = nullptr, const int id = -1);
	virtual ~Event();
	
	std::string getName() const;
	EventType getType() const;
	int getId() const;
	WindowRef getWindow() const;

	double get1DData();
	G3DLite::Vector2 get2DData();
	G3DLite::Vector3 get3DData();
	G3DLite::Vector4 get4DData();
	G3DLite::CoordinateFrame getCoordinateFrameData();
	std::string getMsgData();
	boost::posix_time::ptime getTimestamp();

	bool operator<(Event other) const;
	bool operator<(EventRef otherRef) const;
    
	std::string toString();

	void rename(const std::string &newname);

	void serialize(G3DLite::BinaryOutput &b) const;
	void deserialize(G3DLite::BinaryInput &b);

protected:
	std::string _name;
	int	_id;
	WindowRef _window;
	boost::posix_time::ptime _timestamp;
	EventType _type;
	double _data1D;
	G3DLite::Vector2 _data2D;
	G3DLite::Vector3 _data3D;
	G3DLite::Vector4 _data4D;
	G3DLite::CoordinateFrame _dataCF;
	std::string _dataMsg;
};


} // end namespace

#endif

