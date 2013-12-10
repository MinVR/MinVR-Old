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

#include "MVRCore/Event.H"
#include "MVRCore/StringUtils.H"
#include <boost/format.hpp>

#define BOOST_ASSERT_MSG_OSTREAM std::cout
#include <boost/assert.hpp>

#include "MVRCore/AbstractWindow.H"

namespace MinVR {
	
Event::Event(const std::string &name, const WindowRef window/*= nullptr*/, const int id/*= -1*/)
{ 
	_name = name;
	_type = EVENTTYPE_STANDARD;
	_timestamp = boost::posix_time::microsec_clock::local_time();
	_id = id;
	_window = window;
}

Event::Event(const std::string &name, const double data, const WindowRef window/*= nullptr*/, const int id/*= -1*/)
{ 
	_name = name;
	_data1D = data;
	_type = EVENTTYPE_1D;
	_timestamp = boost::posix_time::microsec_clock::local_time();
	_id = id;
	_window = window;
}

Event::Event(const std::string &name, const glm::vec2 &data, const WindowRef window/*= nullptr*/, const int id/*= -1*/)
{ 
	_name = name;
	_data2D = data;
	_type = EVENTTYPE_2D;
	_timestamp = boost::posix_time::microsec_clock::local_time();
	_id = id;
	_window = window;
}

Event::Event(const std::string &name, const glm::vec3 &data, const WindowRef window/*= nullptr*/, const int id/*= -1*/) 
{ 
	_name = name;
	_data3D = data;
	_type = EVENTTYPE_3D;
	_timestamp = boost::posix_time::microsec_clock::local_time();
	_id = id;
	_window = window;
}

Event::Event(const std::string &name, const glm::vec4 &data, const WindowRef window/*= nullptr*/, const int id/*= -1*/) 
{ 
	_name = name;
	_data4D = data;
	_type = EVENTTYPE_4D;
	_timestamp = boost::posix_time::microsec_clock::local_time();
	_id = id;
	_window = window;
}


Event::Event(const std::string &name, const glm::mat4 &data, const WindowRef window/*= nullptr*/, const int id/*= -1*/) 
{ 
	_name = name;
	_dataCF = data;
	_type = EVENTTYPE_COORDINATEFRAME;
	_timestamp = boost::posix_time::microsec_clock::local_time();
	_id = id;
	_window = window;
}

Event::Event(const std::string &name, const std::string &data, const WindowRef window/*= nullptr*/, const int id/*= -1*/)
{ 
	_name = name;
	_dataMsg = data;
	_type = EVENTTYPE_MSG;
	_timestamp = boost::posix_time::microsec_clock::local_time();
	_id = id;
	_window = window;
}

Event::~Event()
{
}

void Event::rename(const std::string &newname)
{
	_name = newname;
}

std::string Event::getName() const
{
	return _name;
}

Event::EventType Event::getType() const 
{
	return _type; 
}

int Event::getId() const 
{
	return _id; 
}

WindowRef Event::getWindow() const
{ 
	return _window;
}

double Event::get1DData()
{
	return _data1D;
}

glm::vec2 Event::get2DData()
{
	return _data2D;
}

glm::vec3	Event::get3DData()
{
	return _data3D;
}

glm::vec4	Event::get4DData()
{
	return _data4D;
}

glm::mat4	Event::getCoordinateFrameData()
{
	return _dataCF;
}

std::string	Event::getMsgData()
{
	return _dataMsg;
}

boost::posix_time::ptime Event::getTimestamp()
{
	return _timestamp;
}

bool Event::operator<(Event other) const
{
	return _timestamp < other.getTimestamp();
}

bool Event::operator<(EventRef otherRef) const
{
	return _timestamp < otherRef->getTimestamp();
}

std::string	Event::toString()
{
	switch (_type) {
	case EVENTTYPE_1D:
		return boost::str(boost::format("%s (Data: %.2f; Id: %d; Window ptr: %s)") % _name.c_str() % _data1D % _id % _window);
		break;
	case EVENTTYPE_2D:
		return boost::str(boost::format("%s (Data: %.2f,%.2f; Id: %d; Window ptr: %s)") % _name.c_str() % _data2D[0] % _data2D[1] % _id % _window);
		break;
	case EVENTTYPE_3D:
		return boost::str(boost::format("%s (Data: %.2f,%.2f,%.2f; Id: %d; Window ptr: %s)") % _name.c_str() % _data3D[0] % _data3D[1] % _data3D[2] % _id % _window);
		break;
	case EVENTTYPE_4D:
		return boost::str(boost::format("%s (Data: %.2f,%.2f,%.2f,%.2f; Id: %d; Window ptr: %s)") % _name.c_str() % _data4D[0] % _data4D[1] % _data4D[2] % _data4D[3] % _id % _window);
		break;
	case EVENTTYPE_COORDINATEFRAME:
		return boost::str(boost::format("%s (Data: ((%.2f, %.2f, %.2f, %.2f), (%.2f, %.2f, %.2f, %.2f), (%.2f, %.2f, %.2f, %.2f), (%.2f, %.2f, %.2f, %.2f)); Id: %d; Window ptr: %s)") % _name.c_str() % _dataCF[0][0] % _dataCF[0][1] % _dataCF[0][2]  % _dataCF[0][2]
							 % _dataCF[1][0]  % _dataCF[1][1]  % _dataCF[1][2]  % _dataCF[1][2] % _dataCF[2][0] % _dataCF[2][1] % _dataCF[2][2] % _dataCF[2][3] % _dataCF[3][0] % _dataCF[3][1] % _dataCF[3][2] % _dataCF[3][3] % _id % _window);
	case EVENTTYPE_MSG:
		return boost::str(boost::format("%s (Data: %s; Id: %d; Window ptr: %s)") % _name.c_str() % _dataMsg % _id % _window);
		break;
	default:
		return _name;
		break;
	}
}


EventRef createCopyOfEvent(EventRef e)
{
	switch (e->getType()) {
		case Event::EVENTTYPE_STANDARD:
			return std::shared_ptr<Event>(new Event(e->getName(), e->getWindow(), e->getId()));
			break;
		case Event::EVENTTYPE_1D:
			return std::shared_ptr<Event>(new Event(e->getName(),e->get1DData(), e->getWindow(), e->getId()));
			break;
		case Event::EVENTTYPE_2D:
			return std::shared_ptr<Event>(new Event(e->getName(),e->get2DData(), e->getWindow(), e->getId()));
			break;
		case Event::EVENTTYPE_3D:
			return std::shared_ptr<Event>(new Event(e->getName(),e->get3DData(), e->getWindow(), e->getId()));
			break;
		case Event::EVENTTYPE_4D:
			return std::shared_ptr<Event>(new Event(e->getName(),e->get4DData(), e->getWindow(), e->getId()));
			break;
		case Event::EVENTTYPE_COORDINATEFRAME:
			return std::shared_ptr<Event>(new Event(e->getName(),e->getCoordinateFrameData(), e->getWindow(), e->getId()));
			break;
		case Event::EVENTTYPE_MSG:
			return std::shared_ptr<Event>(new Event(e->getName(),e->getMsgData(), e->getWindow(), e->getId()));
			break;
		default:
			BOOST_ASSERT_MSG(false, "createCopyOfEvent: Unknown event type!");
			return NULL;
			break;
	}
}


} // end namespace


