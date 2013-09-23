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

// only compile if USE_VRPN is defined!
#ifdef USE_VRPN

// Note: This include ordering is important, don't screw with it!
#include "MVRCore/InputDeviceVRPNButton.H"
#include <vrpn_Button.h>


#ifndef VRPN_CALLBACK
#define VRPN_CALLBACK
#endif

namespace MinVR {


void  VRPN_CALLBACK	buttonHandler(void *thisPtr, const vrpn_BUTTONCB info)
{
	((InputDeviceVRPNButton*)thisPtr)->sendEvent(info.button, info.state);
}



InputDeviceVRPNButton::InputDeviceVRPNButton(const std::string &vrpnButtonDeviceName, const std::vector<std::string> &eventsToGenerate)
{
	_eventNames = eventsToGenerate;

	_vrpnDevice = new vrpn_Button_Remote(vrpnButtonDeviceName.c_str());
	if (!_vrpnDevice) {
		std::stringstream ss;
		ss << "Can't create VRPN Remote Button with name " + vrpnButtonDeviceName;
		BOOST_ASSERT_MSG(false, ss.str().c_str());
	}

	_vrpnDevice->register_change_handler(this, buttonHandler);
}

InputDeviceVRPNButton::InputDeviceVRPNButton(const std::string name, const ConfigMapRef map)
{
	std::string vrpnname = map->get( name + "_InputDeviceVRPNButtonName", "" );
	std::string events   = map->get( name + "_EventsToGenerate","" );

	BOOST_LOG_TRIVIAL(info) << "Creating new InputDeviceVRPNButton (" + vrpnname + ")";

	_eventNames = splitStringIntoArray( events );

	_vrpnDevice = new vrpn_Button_Remote(vrpnname.c_str());
	if (!_vrpnDevice) {
		std::stringstream ss;
		ss << "Can't create VRPN Remote Button with name " + vrpnname;
		BOOST_ASSERT_MSG(false, ss.str().c_str());
	}

	_vrpnDevice->register_change_handler(this, buttonHandler);
}

InputDeviceVRPNButton::~InputDeviceVRPNButton()
{
}

std::string InputDeviceVRPNButton::getEventName(int buttonNumber)
{
	if (buttonNumber >= _eventNames.size()) {
		return std::string("VRPNButtonDevice_Unknown_Event");
	}
	else {
		return _eventNames[buttonNumber];
	}
}

void InputDeviceVRPNButton::sendEvent(int buttonNumber, bool down)
{
	std::string ename = getEventName(buttonNumber);
	if (down) {
		_pendingEvents.push_back(EventRef(new Event(ename + "_down", nullptr, buttonNumber)));
	}
	else {
		_pendingEvents.push_back(EventRef(new Event(ename + "_up", nullptr, buttonNumber)));
	}
}

void InputDeviceVRPNButton::pollForInput(std::vector<EventRef> &events)
{
	_vrpnDevice->mainloop();
	if (_pendingEvents.size()) {
		for(auto pending_it = _pendingEvents.begin(); pending_it != _pendingEvents.end(); ++ pending_it) {
			events.push_back(*pending_it);
		}
		_pendingEvents.clear();
	}
}


} // end namespace


#endif // USE_VRPN

