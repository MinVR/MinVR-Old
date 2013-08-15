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

/**
   \author     Daniel Keefe (dfk)
   \maintainer Daniel Keefe (dfk)
   
   \created 2004-01-28
   \edited  2004-01-28
   
   \file  VRPNButtonDevice.H
   
   \brief A driver that connects to VRPN and creates Events
          based on button data from VRPN.
*/

#ifndef VRPNBUTTONDEVICE_H
#define VRPNBUTTONDEVICE_H

// Note: This include ordering is important, don't screw with it!
#include <G3DLite/G3DLite.h>
#include <iostream>

#include "MVRCore/AbstractInputDevice.H"
#include "MVRCore/ConfigMap.H"

#ifdef USE_VRPN
class vrpn_Button_Remote;
#endif

namespace MinVR {

/**
*/
class InputDeviceVRPNButton : public AbstractInputDevice
{
public:
#ifdef USE_VRPN
	InputDeviceVRPNButton(const std::string &vrpnButtonDeviceName, const G3DLite::Array<std::string> &eventsToGenerate);
	InputDeviceVRPNButton(const std::string name, G3DLite::Log* log, const ConfigMapRef map);

	virtual ~InputDeviceVRPNButton();

	void pollForInput(G3DLite::Array<EventRef> &events);

	std::string getEventName(int buttonNumber);
	void sendEvent(int buttonNumber, bool down);

private:
	vrpn_Button_Remote  *_vrpnDevice;
	G3DLite::Array<std::string>   _eventNames;
	G3DLite::Array<EventRef>      _pendingEvents;
#else
	InputDeviceVRPNButton(const std::string &vrpnButtonDeviceName, const G3DLite::Array<std::string> &eventsToGenerate)
	{
		alwaysAssertM(false, "VRPN is currently unsupported. Set cmake option USE_VRPN to ON and recompile.");
	}
	InputDeviceVRPNButton(const std::string name, G3DLite::Log* log, const ConfigMapRef map)
	{
		alwaysAssertM(false, "VRPN is currently unsupported. Set cmake option USE_VRPN to ON and recompile.");
	}

	virtual ~InputDeviceVRPNButton() {}
	void pollForInput(G3DLite::Array<EventRef> &events) {}
#endif // USE_VRPN defined
};

} // end namespace

#endif




