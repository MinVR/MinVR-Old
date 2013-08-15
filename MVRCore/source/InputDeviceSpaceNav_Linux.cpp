//========================================================================
// MinVR
// Platform:    Linux
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

#ifdef USE_SPACENAV

#include  <iostream>
#include  <stdio.h>
#include  <stdlib.h>
#include  <signal.h>
#include  <spnav.h> 

#include  "MVRCore/InputDeviceSpaceNav.H"

using namespace std;
using namespace G3DLite;

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

void InputDeviceSpaceNav::pollForInput(Array<EventRef> &events) {
	//Just sayin, the open source library is way nicer than the closed
	if(connected){
		spnav_event sev;
		while(spnav_poll_event(&sev)) {//non-blocking API
			if(sev.type == SPNAV_EVENT_MOTION) {
				Vector3 trans(-sev.motion.x, sev.motion.z, sev.motion.y);
				Vector3 rot(-sev.motion.rx, sev.motion.rz, sev.motion.ry);
				trans=trans/-400.0;//roughly normalizes
				rot=rot/-400.0;//roughly normalizes
				events.append(new MinVR::Event("SpaceNav_Trans", trans));
				events.append(new MinVR::Event("SpaceNav_Rot", rot));
			} else {	/* SPNAV_EVENT_BUTTON */
				if(sev.button.press) {
					if(sev.button.bnum==0) 
						events.append(new MinVR::Event("SpaceNav_Btn1_down"));
					else
						events.append(new MinVR::Event("SpaceNav_Btn2_down"));
				}else{
					if(sev.button.bnum==0) 
						events.append(new MinVR::Event("SpaceNav_Btn1_up"));
					else
						events.append(new MinVR::Event("SpaceNav_Btn2_up"));
				}
			}
		}
	}
}


} // end namespace

#endif // use_spacenav