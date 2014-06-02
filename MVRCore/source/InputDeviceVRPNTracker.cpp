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

// only compile if USE_VRPN is defined!!
#ifdef USE_VRPN

// Note: This include ordering is important, don't screw with it!
#include "MVRCore/InputDeviceVRPNTracker.H"

#include <vrpn_Tracker.h>

#include <iostream>
using namespace std;

namespace MinVR {


#ifndef VRPN_CALLBACK
#define VRPN_CALLBACK
#endif


// Callback function for VRPN, void* pointer points to a VRPNTrackerDevice
void VRPN_CALLBACK trackerHandler(void *thisPtr, const vrpn_TRACKERCB info)
{
	glm::dmat4 vrpnEvent;
	vrpnEvent = glm::mat4_cast(glm::quat(info.quat[3],info.quat[0], info.quat[1],info.quat[2]));
	vrpnEvent = glm::column(vrpnEvent, 3, glm::dvec4(info.pos[0],info.pos[1],info.pos[2], 1.0));

	InputDeviceVRPNTracker* device = ((InputDeviceVRPNTracker*)thisPtr);
	boost::posix_time::ptime msgTime = boost::posix_time::microsec_clock::local_time();
	device->processEvent(vrpnEvent, info.sensor, msgTime);
}

InputDeviceVRPNTracker::InputDeviceVRPNTracker(
	const std::string            &vrpnTrackerDeviceName,
	const std::vector<std::string>     &eventsToGenerate,
	const double                 &trackerUnitsToRoomUnitsScale,
	const glm::dmat4        &deviceToRoom,
	const std::vector<glm::dmat4> &propToTracker,
	const std::vector<glm::dmat4> &finalOffset,
	const bool                   &waitForNewReportInPoll,
	const bool                   &convertLHtoRH,
	const bool                   &ignoreZeroes)
{
	_eventNames                   = eventsToGenerate;
	_trackerUnitsToRoomUnitsScale = trackerUnitsToRoomUnitsScale;
	_deviceToRoom                 = deviceToRoom;
	_propToTracker                = propToTracker;
	_finalOffset                  = finalOffset;
	_waitForNewReport             = waitForNewReportInPoll;
	_convertLHtoRH                = convertLHtoRH;
	_ignoreZeroes                 = ignoreZeroes;
	_printSensor0                 = false;

	_vrpnDevice = new vrpn_Tracker_Remote(vrpnTrackerDeviceName.c_str());
	std::stringstream ss;
	ss << "Can't create VRPN Remote Tracker with name " + vrpnTrackerDeviceName;
	BOOST_ASSERT_MSG(_vrpnDevice, ss.str().c_str());
	_vrpnDevice->register_change_handler(this, trackerHandler);
}

InputDeviceVRPNTracker::InputDeviceVRPNTracker( const std::string name, const ConfigMapRef map)
{
	std::string vrpnname  = map->get( name + "_InputDeviceVRPNTrackerName", "" );
	std::string eventsStr = map->get( name + "_EventsToGenerate", "" );
	std::vector<std::string> events = splitStringIntoArray(eventsStr);

	double scale = map->get( name + "_TrackerUnitsToRoomUnitsScale", 1.0 );
	glm::dmat4 d2r = map->get( name + "_DeviceToRoom", glm::dmat4(1.0) );
	
	// orthonormalize
	glm::dmat3 rot(d2r[0][0], d2r[0][1], d2r[0][2],
				  d2r[1][0], d2r[1][1], d2r[1][2],
				  d2r[2][0], d2r[2][1], d2r[2][2]);
	rot = glm::orthonormalize(rot);
	d2r[0][0] = rot[0][0];
	d2r[0][1] = rot[0][1];
	d2r[0][2] = rot[0][2];
	d2r[1][0] = rot[1][0];
	d2r[1][1] = rot[1][1];
	d2r[1][2] = rot[1][2];
	d2r[2][0] = rot[2][0];
	d2r[2][1] = rot[2][1];
	d2r[2][2] = rot[2][2];

	std::vector<glm::dmat4> p2t;
	std::vector<glm::dmat4> fo;

	for (int  i = 0; i < events.size(); i++)
	{
		glm::dmat4 cf = map->get(events[i] + "_PropToTracker", glm::dmat4(1.0));
		glm::dmat3 rot(cf[0][0], cf[0][1], cf[0][2],
				  cf[1][0], cf[1][1], cf[1][2],
				  cf[2][0], cf[2][1], cf[2][2]);
		rot = glm::orthonormalize(rot);
		cf[0][0] = rot[0][0];
		cf[0][1] = rot[0][1];
		cf[0][2] = rot[0][2];
		cf[1][0] = rot[1][0];
		cf[1][1] = rot[1][1];
		cf[1][2] = rot[1][2];
		cf[2][0] = rot[2][0];
		cf[2][1] = rot[2][1];
		cf[2][2] = rot[2][2];

		p2t.push_back(cf);
		glm::dmat4 cf2 = map->get(events[i] + "_FinalOffset", glm::dmat4(1.0));
		glm::dmat3 rot2(cf2[0][0], cf2[0][1], cf2[0][2],
				  cf2[1][0], cf2[1][1], cf2[1][2],
				  cf2[2][0], cf2[2][1], cf2[2][2]);
		rot2 = glm::orthonormalize(rot2);
		cf2[0][0] = rot2[0][0];
		cf2[0][1] = rot2[0][1];
		cf2[0][2] = rot2[0][2];
		cf2[1][0] = rot2[1][0];
		cf2[1][1] = rot2[1][1];
		cf2[1][2] = rot2[1][2];
		cf2[2][0] = rot2[2][0];
		cf2[2][1] = rot2[2][1];
		cf2[2][2] = rot2[2][2];
		fo.push_back(cf2);
	}

	bool wait          = map->get( name + "_WaitForNewReportInPoll", false );
	bool convertLHtoRH = map->get( name + "_ConvertLHtoRH", false );
	bool ignoreZeroes  = map->get( name + "_IgnoreZeroes", false );

	boost::log::sources::logger logger;
	logger.add_attribute("Tag", boost::log::attributes::constant< std::string >("MinVR Core"));
	BOOST_LOG(logger) << "Creating new InputDeviceVRPNTracker ( " + vrpnname + ")";

	_eventNames                   = events;
	_trackerUnitsToRoomUnitsScale = scale;
	_deviceToRoom                 = d2r;
	_propToTracker                = p2t;
	_finalOffset                  = fo;
	_waitForNewReport             = wait;
	_convertLHtoRH                = convertLHtoRH;
	_ignoreZeroes                 = ignoreZeroes;
	_printSensor0                 = false;

	_vrpnConnection = vrpn_get_connection_by_name(vrpnname.c_str());
	_vrpnDevice = new vrpn_Tracker_Remote(vrpnname.c_str(), _vrpnConnection);
	std::stringstream ss;
	ss <<  "Can't create VRPN Remote Tracker with name " + vrpnname;
	BOOST_ASSERT_MSG(_vrpnDevice, ss.str().c_str());
	_vrpnDevice->register_change_handler(this, trackerHandler);
}


InputDeviceVRPNTracker::~InputDeviceVRPNTracker()
{
}

/**
The tracker reports the transformation that would transform stuff
in tracker space to device space (that is, to the space relative to
the origin of the tracking device).  First we change the units on
the translation part of this since we usually work in feet and most
tracking systems report mesurements in some metric system.  Then we
have a transformation in feet relative to the device.  We multiply
by a transformation that transforms stuff in device space to IS3D
room space.  You can think of this as what rotation, then
translation would move the origin of RoomSpace to the origin of
tracking device.  This is the deviceToRoom coordinate frame.
*/
void InputDeviceVRPNTracker::processEvent(const glm::dmat4 &vrpnEvent, int sensorNum, const boost::posix_time::ptime &msg_time)
{

	if(_ignoreZeroes && glm::column(vrpnEvent, 3) == glm::dvec4(0.0, 0.0, 0.0, 1.0)){
		return;
	}
	_newReportFlag = true;

	// first, adjust units of trackerToDevice.  after this, everything
	// is in RoomSpace units (typically feet for VRG3D).
	glm::dmat4 trackerToDevice = vrpnEvent;

	// convert a left handed coordinate system to a right handed one
	// not sure if this actually works..
	if (_convertLHtoRH) {
		/*** This is code from syzygy..
		CoordinateFrame switchMat(Matrix3(1,0,0, 0,0,1, 0,-1,0), Vector3::zero());
		CoordinateFrame transMat(switchMat.inverse() * vrpnEvent.translation);
		CoordinateFrame rotMat(vrpnEvent.rotation, Vector3::zero());
		trackerToDevice = transMat * switchMat.inverse() * rotMat * switchMat;
		***/

		// This code is based on the article "Conversion of Left-Handed
		// Coordinates to Right-Handed Coordinates" by David Eberly,
		// available online:
		// http://www.geometrictools.com/Documentation/LeftHandedToRightHanded.pdf
		trackerToDevice[3][2] = -trackerToDevice[3][2];

		trackerToDevice[2][0] = -trackerToDevice[2][0];
		trackerToDevice[2][1] = -trackerToDevice[2][1];
		trackerToDevice[0][2] = -trackerToDevice[0][2];
		trackerToDevice[1][2] = -trackerToDevice[1][2];
	}

	trackerToDevice[3][0] *= _trackerUnitsToRoomUnitsScale;
	trackerToDevice[3][1] *= _trackerUnitsToRoomUnitsScale;
	trackerToDevice[3][2] *= _trackerUnitsToRoomUnitsScale;

	glm::dmat4 eventRoom = _finalOffset[sensorNum] * _deviceToRoom * trackerToDevice * _propToTracker[sensorNum];

	if ((_printSensor0) && (sensorNum == 0)) {
		glm::dvec4 translation = glm::column(eventRoom, 3);
		std::cout << translation << std::endl;
	}
	_pendingEvents.push_back(EventRef(new Event(getEventName(sensorNum), eventRoom, nullptr, sensorNum, msg_time)));
}

std::string InputDeviceVRPNTracker::getEventName(int trackerNumber)
{
	if (trackerNumber >= _eventNames.size())
		return std::string("VRPNTrackerDevice_Unknown_Event");
	else
		return _eventNames[trackerNumber];
}

void InputDeviceVRPNTracker::pollForInput(std::vector<EventRef> &events)
{
	// If this poll routine isn't called fast enough then the UDP buffer can fill up and
	// the most recent tracker records will be dropped, introducing lag in the system.
	// A workaround suggested by the VRPN website is to keep calling mainloop() until you
	// get a new tracker report.  This should only really be an issue if your framerate
	// is low.
	if (_waitForNewReport) {
		_newReportFlag = false;
		while (!_newReportFlag) {
			_vrpnDevice->mainloop();
		}
	}
	else {
		_vrpnDevice->mainloop();
	}

	for(auto pending_it=_pendingEvents.begin(); pending_it != _pendingEvents.end(); ++pending_it) {
		events.push_back(*pending_it);
	}
	_pendingEvents.clear();
}

} // end namespace


#endif  // USE_VRPN

