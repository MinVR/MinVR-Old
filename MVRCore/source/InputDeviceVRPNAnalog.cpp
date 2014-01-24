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

#ifdef USE_VRPN

// Note: This include ordering is important, don't screw with it!
#include "MVRCore/InputDeviceVRPNAnalog.H"
#include "MVRCore/Event.H"
#include <vrpn_Analog.h>


#ifndef VRPN_CALLBACK
#define VRPN_CALLBACK
#endif

namespace MinVR {

void VRPN_CALLBACK analogHandler(void *thisPtr, const vrpn_ANALOGCB info)
{
	int lastchannel = (int)glm::min(info.num_channel, (int)((InputDeviceVRPNAnalog*)thisPtr)->numChannels());
	boost::posix_time::ptime msgTime = boost::posix_time::microsec_clock::local_time();
	for (int i=0;i<lastchannel;i++) {
		((InputDeviceVRPNAnalog*)thisPtr)->sendEventIfChanged(i, info.channel[i], msgTime);
	}
}

InputDeviceVRPNAnalog::InputDeviceVRPNAnalog(const std::string &vrpnAnalogDeviceName, const std::vector<std::string> &eventsToGenerate)
{
	_eventNames = eventsToGenerate;
	for (int i=0;i<_eventNames.size();i++) {
		_channelValues.push_back(0.0);
	}

	_vrpnDevice = new vrpn_Analog_Remote(vrpnAnalogDeviceName.c_str());
	if (!_vrpnDevice) {
		std::stringstream ss;
		ss << "Can't create VRPN Remote Analog with name" + vrpnAnalogDeviceName;
		BOOST_ASSERT_MSG(false, ss.str().c_str());
	}

	_vrpnDevice->register_change_handler(this, analogHandler);
}

InputDeviceVRPNAnalog::InputDeviceVRPNAnalog(const std::string name, const ConfigMapRef map)
{
	std::string  vrpnname = map->get( name + "_InputDeviceVRPNAnalogName", "" );
	std::string  events   = map->get( name + "_EventsToGenerate", "" );

	BOOST_LOG_TRIVIAL(info) << "Creating new InputDeviceVRPNAnalog (" + vrpnname + ")";

	_eventNames = splitStringIntoArray( events );
	for (int i=0;i<_eventNames.size();i++) { 
		_channelValues.push_back(0.0);
	}

	_vrpnDevice = new vrpn_Analog_Remote(vrpnname.c_str());
	if (!_vrpnDevice) { 
		std::stringstream ss;
		ss << "Can't create VRPN Remote Analog with name" + vrpnname;
		BOOST_ASSERT_MSG(false, ss.str().c_str());
	}

	_vrpnDevice->register_change_handler(this, analogHandler);
}

InputDeviceVRPNAnalog::~InputDeviceVRPNAnalog()
{
}

std::string	InputDeviceVRPNAnalog::getEventName(int channelNumber)
{
	if (channelNumber >= _eventNames.size()) {
		return std::string("VRPNAnalogDevice_Unknown_Event");
	}
	else {
		return _eventNames[channelNumber];
	}
}

void InputDeviceVRPNAnalog::sendEventIfChanged(int channelNumber, double data, const boost::posix_time::ptime &msg_time)
{
	if (_channelValues[channelNumber] != data) {
		_pendingEvents.push_back(EventRef(new Event(_eventNames[channelNumber], data, nullptr, channelNumber, msg_time)));
		_channelValues[channelNumber] = data;
	}
}

void InputDeviceVRPNAnalog::pollForInput(std::vector<EventRef> &events)
{
	_vrpnDevice->mainloop();
	if (_pendingEvents.size()) {
		for(auto pending_it=_pendingEvents.begin(); pending_it != _pendingEvents.end(); ++ pending_it) {
			events.push_back(*pending_it);
		}
		_pendingEvents.clear();
	}
}


} // end namespace


#endif // USE_VRPN

