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

#include "MVRCore/Event.H"
#include "MVRCore/StringUtils.H"

using namespace G3DLite;

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

Event::Event(const std::string &name, const G3DLite::Vector2 &data, const WindowRef window/*= nullptr*/, const int id/*= -1*/)
{ 
	_name = name;
	_data2D = data;
	_type = EVENTTYPE_2D;
	_timestamp = boost::posix_time::microsec_clock::local_time();
	_id = id;
	_window = window;
}

Event::Event(const std::string &name, const G3DLite::Vector3 &data, const WindowRef window/*= nullptr*/, const int id/*= -1*/) 
{ 
	_name = name;
	_data3D = data;
	_type = EVENTTYPE_3D;
	_timestamp = boost::posix_time::microsec_clock::local_time();
	_id = id;
	_window = window;
}

Event::Event(const std::string &name, const G3DLite::Vector4 &data, const WindowRef window/*= nullptr*/, const int id/*= -1*/) 
{ 
	_name = name;
	_data4D = data;
	_type = EVENTTYPE_4D;
	_timestamp = boost::posix_time::microsec_clock::local_time();
	_id = id;
	_window = window;
}


Event::Event(const std::string &name, const G3DLite::CoordinateFrame &data, const WindowRef window/*= nullptr*/, const int id/*= -1*/) 
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

Vector2	Event::get2DData()
{
	return _data2D;
}

Vector3	Event::get3DData()
{
	return _data3D;
}

Vector4	Event::get4DData()
{
	return _data4D;
}

CoordinateFrame	Event::getCoordinateFrameData()
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
		return format("%s (Data: %.2f; Id: %d; Window ptr: %s)", _name.c_str(), _data1D, _id, _window);
		break;
	case EVENTTYPE_2D:
		return format("%s (Data: %.2f,%.2f; Id: %d; Window ptr: %s)", _name.c_str(), _data2D[0], _data2D[1]);
		break;
	case EVENTTYPE_3D:
		return format("%s (Data: %.2f,%.2f,%.2f; Id: %d; Window ptr: %s)", _name.c_str(), _data3D[0], _data3D[1], _data3D[2]);
		break;
	case EVENTTYPE_4D:
		return format("%s (Data: %.2f,%.2f,%.2f,%.2f; Id: %d; Window ptr: %s)", _name.c_str(), _data4D[0], _data4D[1], _data4D[2], _data4D[3]);
		break;
	case EVENTTYPE_COORDINATEFRAME:
		return format("%s (Data: %s; Id: %d; Window ptr: %s)", _name.c_str(), matrix4ToString(_dataCF.toMatrix4()), _id, _window);
	case EVENTTYPE_MSG:
		return format("%s (Data: %s; Id: %d; Window ptr: %s)", _name.c_str(), _dataMsg, _id, _window);
		break;
	default:
		return _name;
		break;
	}
}


void Event::serialize(BinaryOutput &b) const
{
	b.writeInt32(_name.size());
	b.writeString(_name);
	b.writeInt8((int8)_type);
	switch (_type) {
	case EVENTTYPE_STANDARD:
		break;
	case EVENTTYPE_1D: {
		float64 d = _data1D;
		b.writeFloat64(d);
		break;
						}
	case EVENTTYPE_2D:
		b.writeVector2(_data2D);
		break;
	case EVENTTYPE_3D:
		b.writeVector3(_data3D);
		break;
	case EVENTTYPE_4D:
		b.writeVector4(_data4D);
		break;
	case EVENTTYPE_COORDINATEFRAME:
		_dataCF.serialize(b);
		break;
	case EVENTTYPE_MSG:
		b.writeInt32(_dataMsg.size());
		b.writeString(_dataMsg);
		break;
	}
	b.writeInt32(_id);
}

void Event::deserialize(BinaryInput &b)
{
	int size = b.readInt32();
	_name    = b.readString(size+1);
	_type    = (EventType)b.readInt8();
	switch (_type) {
	case EVENTTYPE_STANDARD:
		break;
	case EVENTTYPE_1D:
		_data1D  = b.readFloat64();
		break;
	case EVENTTYPE_2D:
		_data2D  = b.readVector2();
		break;
	case EVENTTYPE_3D:
		_data3D  = b.readVector3();
		break;
	case EVENTTYPE_4D:
		_data4D  = b.readVector4();
		break;
	case EVENTTYPE_COORDINATEFRAME:
		_dataCF.deserialize(b);
		_dataCF.rotation.orthonormalize();
		break;
	case EVENTTYPE_MSG: {
		int size = b.readInt32();
		_dataMsg = b.readString(size+1);
		break;
						}
	default:
		// I got to this with windows server/client at Duke's Cave!!
		alwaysAssertM(false, "Error, unknown eventtype in deserialize method.");
		break;
	}
	_id = b.readInt32();
}

EventRef createCopyOfEvent(EventRef e)
{
	switch (e->getType()) {
		case Event::EVENTTYPE_STANDARD:
			return new Event(e->getName(), e->getWindow(), e->getId());
			break;
		case Event::EVENTTYPE_1D:
			return new Event(e->getName(),e->get1DData(), e->getWindow(), e->getId());
			break;
		case Event::EVENTTYPE_2D:
			return new Event(e->getName(),e->get2DData(), e->getWindow(), e->getId());
			break;
		case Event::EVENTTYPE_3D:
			return new Event(e->getName(),e->get3DData(), e->getWindow(), e->getId());
			break;
		case Event::EVENTTYPE_4D:
			return new Event(e->getName(),e->get4DData(), e->getWindow(), e->getId());
			break;
		case Event::EVENTTYPE_COORDINATEFRAME:
			return new Event(e->getName(),e->getCoordinateFrameData(), e->getWindow(), e->getId());
			break;
		case Event::EVENTTYPE_MSG:
			return new Event(e->getName(),e->getMsgData(), e->getWindow(), e->getId());
			break;
		default:
			alwaysAssertM(false, "createCopyOfEvent: Unknown event type!");
			return NULL;
			break;
	}
}


} // end namespace


