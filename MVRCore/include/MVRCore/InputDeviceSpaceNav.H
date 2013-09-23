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
#include "MVRCore/AbstractInputDevice.H"
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
