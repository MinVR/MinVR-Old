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
\author Daniel Keefe (dfk)
\file   SpaceNavDevice.h
\brief  Driver for the SpaceNavigator device made by 3DConnexion.
This also works for other similar devices, like the SpacePilot,
although it only supports 2 buttons and 6DOF input from the
hockey puck thingy.

OSX Setup: The current implementation only works on OSX and
requires you to install 3DConnexion's SDK framework, which
is packaged with their latest OSX driver.  Then, add
"3DconnexionClient" to the FRAMEWORKS variable in your
Makefile to link with the SDK.
*/


#ifndef SPACENAVDEV_H
#define SPACENAVDEV_H

#include <glm/glm.hpp>
#include "MVRCore/AbstractInputDevice.h"
#include "MVRCore/ConfigMap.H"
#include "MVRCore/Event.H"
#include <vector>
#include <boost/log/trivial.hpp>

#define BOOST_ASSERT_MSG_OSTREAM std::cout
#include <boost/assert.hpp>


namespace MinVR {

class InputDeviceSpaceNav : public AbstractInputDevice
{
public:

#ifdef USE_SPACENAV
	InputDeviceSpaceNav( const std::string name, const ConfigMapRef map = NULL )
	{
		BOOST_LOG_TRIVIAL(info) << "Creating new SpaceNavDevice";
		setup();
	}

	void setup();

	virtual ~InputDeviceSpaceNav();

	void pollForInput(std::vector<EventRef> &events);

private:
	bool connected;

#else
	InputDeviceSpaceNav(const std::string name, const ConfigMapRef map = NULL)
	{
		BOOST_ASSERT_MSG(false, "Spacenav is currently unsupported. Set cmake option USE_SPACENAV to ON and recompile.");
	}

	virtual ~InputDeviceSpaceNav() {}
	void pollForInput(std::vector<EventRef> &events) {}

#endif             // USE_SPACENAV
};

}                  // end namespace

#endif             // SPACENAVDEV_H
