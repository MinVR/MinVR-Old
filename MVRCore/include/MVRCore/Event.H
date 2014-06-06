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

#ifndef MINVREVENT_H
#define MINVREVENT_H

#include <glm/glm.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "MVRCore/StringUtils.H"

namespace MinVR {

class AbstractWindow;
typedef std::shared_ptr<class AbstractWindow> WindowRef;

class Event;
typedef std::shared_ptr<class Event> EventRef;

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

	Event(const std::string &name, const WindowRef window = nullptr, const int id = -1, const boost::posix_time::ptime &timestamp = boost::posix_time::ptime(boost::posix_time::not_a_date_time));
	Event(const std::string &name, const double data, const WindowRef window = nullptr, const int id = -1, const boost::posix_time::ptime &timestamp = boost::posix_time::ptime(boost::posix_time::not_a_date_time));
	Event(const std::string &name, const glm::dvec2 &data, const WindowRef window = nullptr, const int id = -1, const boost::posix_time::ptime &timestamp = boost::posix_time::ptime(boost::posix_time::not_a_date_time));
	Event(const std::string &name, const glm::dvec3 &data, const WindowRef window = nullptr, const int id = -1, const boost::posix_time::ptime &timestamp = boost::posix_time::ptime(boost::posix_time::not_a_date_time));
	Event(const std::string &name, const glm::dvec4 &data, const WindowRef window = nullptr, const int id = -1, const boost::posix_time::ptime &timestamp = boost::posix_time::ptime(boost::posix_time::not_a_date_time));
	Event(const std::string &name, const glm::dmat4 &data, const WindowRef window = nullptr, const int id = -1, const boost::posix_time::ptime &timestamp = boost::posix_time::ptime(boost::posix_time::not_a_date_time));
	Event(const std::string &name, const std::string &data, const WindowRef window = nullptr, const int id = -1, const boost::posix_time::ptime &timestamp = boost::posix_time::ptime(boost::posix_time::not_a_date_time));
	Event(const std::string &eventString, const boost::posix_time::ptime &timestamp); // Create an event from a string in the format of Event::toString();
	virtual ~Event();
	
	std::string getName() const;
	EventType getType() const;
	int getId() const;
	WindowRef getWindow() const;

	double get1DData();
	glm::dvec2 get2DData();
	glm::dvec3 get3DData();
	glm::dvec4 get4DData();
	glm::dmat4 getCoordinateFrameData();
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
	glm::dvec2 _data2D;
	glm::dvec3 _data3D;
	glm::dvec4 _data4D;
	glm::dmat4 _dataCF;
	std::string _dataMsg;
};


} // end namespace

#endif

