//========================================================================
// MinVR
// Platform:    Linux
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

#ifdef USE_SPACENAV

#include  <iostream>
#include  <stdio.h>
#include  <stdlib.h>
#include  <signal.h>
#include  <spnav.h> 

#include  "MVRCore/InputDeviceSpaceNav.H"

using namespace std;

namespace MinVR {

void InputDeviceSpaceNav::setup()
{
	connected=true;
	if(spnav_open()==-1) {
		fprintf(stderr, "failed to connect to the space navigator daemon\n");
		fprintf(stderr, "Continuing without SpaceNav Support(note: free spacenavd, not propietary driver, must be used.)\n");
		fprintf(stderr, "driver available at: spacenav.sourceforge.net\n");
		connected=false;
	}
}

InputDeviceSpaceNav::~InputDeviceSpaceNav() {
	spnav_close();
}

void InputDeviceSpaceNav::pollForInput(std::vector<EventRef> &events) {
	//Just sayin, the open source library is way nicer than the closed
	if(connected){
		spnav_event sev;
		while(spnav_poll_event(&sev)) {//non-blocking API
			if(sev.type == SPNAV_EVENT_MOTION) {
				glm::dvec3 trans(-sev.motion.x, sev.motion.z, sev.motion.y);
				glm::dvec3 rot(-sev.motion.rx, sev.motion.rz, sev.motion.ry);
				trans=trans/-400.0;//roughly normalizes
				rot=rot/-400.0;//roughly normalizes
				events.push_back(EventRef(new MinVR::Event("SpaceNav_Trans", trans)));
				events.push_back(EventRef(new MinVR::Event("SpaceNav_Rot", rot)));
			} else {	/* SPNAV_EVENT_BUTTON */
				if(sev.button.press) {
					if(sev.button.bnum==0) 
						events.push_back(EventRef(new MinVR::Event("SpaceNav_Btn1_down")));
					else
						events.push_back(EventRef(new MinVR::Event("SpaceNav_Btn2_down")));
				}else{
					if(sev.button.bnum==0) 
						events.push_back(EventRef(new MinVR::Event("SpaceNav_Btn1_up")));
					else
						events.push_back(EventRef(new MinVR::Event("SpaceNav_Btn2_up")));
				}
			}
		}
	}
}


} // end namespace

#endif // use_spacenav