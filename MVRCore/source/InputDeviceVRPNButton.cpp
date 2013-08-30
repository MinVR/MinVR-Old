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

