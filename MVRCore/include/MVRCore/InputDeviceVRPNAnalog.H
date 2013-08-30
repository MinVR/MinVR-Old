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


#ifndef VRPNANALOGDEVICE_H
#define VRPNANALOGDEVICE_H

// Note: This include ordering is important, don't screw with it!
#include <glm/glm.hpp>
#include <vector>
#include <iostream>

#include "MVRCore/AbstractInputDevice.H"
#include "MVRCore/ConfigMap.H"

#include <boost/log/trivial.hpp>
#define BOOST_ASSERT_MSG_OSTREAM std::cout
#include <boost/assert.hpp>

#ifdef USE_VRPN
class vrpn_Analog_Remote;
#endif

namespace MinVR {

/**
*/
class InputDeviceVRPNAnalog : public AbstractInputDevice
{
public:

#ifdef USE_VRPN
	InputDeviceVRPNAnalog(const std::string &vrpnAnalogDeviceName, const std::vector<std::string> &eventsToGenerate);
	InputDeviceVRPNAnalog(const std::string name, const ConfigMapRef map);

	virtual ~InputDeviceVRPNAnalog();

	void        pollForInput(std::vector<EventRef> &events);
	void        sendEventIfChanged(int channelNumber, double data);
	std::string getEventName(int channelNumber);
	int         numChannels() { return _eventNames.size(); }

private:
	vrpn_Analog_Remote  *_vrpnDevice;
	std::vector<std::string>   _eventNames;
	std::vector<double>        _channelValues;
	std::vector<EventRef>      _pendingEvents;
#else
	InputDeviceVRPNAnalog(const std::string &vrpnAnalogDeviceName, const std::vector<std::string> &eventsToGenerate)
	{
		BOOST_ASSERT_MSG(false, "VRPN is currently unsupported. Set cmake option USE_VRPN to ON and recompile.");
	}
	InputDeviceVRPNAnalog(const std::string name, const ConfigMapRef map)
	{
		BOOST_ASSERT_MSG(false, "VRPN is currently unsupported. Set cmake option USE_VRPN to ON and recompile.");
	}

	virtual ~InputDeviceVRPNAnalog() {};
	void pollForInput(std::vector<EventRef> &events) {};
#endif
};

} // end namespace



#endif


