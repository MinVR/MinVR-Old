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
\file   VRPNTrackerDevice.H
\brief  A driver that connects to VRPN and creates CoordinateFrame Events
based on tracker data from VRPN.
*/

#ifndef VRPNTRACKERDEVICE_H
#define VRPNTRACKERDEVICE_H

// Note: This include ordering is important, don't screw with it!
#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/orthonormalize.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include "MVRCore/AbstractInputDevice.H"
#include "MVRCore/ConfigMap.H"
#include "MVRCore/StringUtils.H"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

#include <boost/log/trivial.hpp>
#define BOOST_ASSERT_MSG_OSTREAM std::cout
#include <boost/assert.hpp>

#ifdef USE_VRPN
class vrpn_Tracker_Remote;
class vrpn_Connection;
#endif

namespace MinVR {

	/**
	Calibration Procedure:

	1. Set TrackerUnitsToRoomUnitsScale to get everything in feet.

	2. Adjust DeviceToRoom by printing out the position of sensor 0
	until it reports the origin of RoomSpace in the correct place and
	+X, +Y, and +Z all point in the correct direction.  You can print
	out the value of sensor 0 by calling printSensor0(true).

	3. Use the TestTrackers function of the IS3D/programTemplate demo
	to draw the CoordinateFrame for each tracker.  Adjust each
	tracker's PropToTracker transformation until it aligns itself with
	the RoomSpace frame when the tracker is held at the origin of
	RoomSpace.

	Here's an easy way to think about it: Hold up each prop at the
	origin of RoomSpace in the orientation that you want to be
	considered "normal".  Then, look at the drawing of its frame.  For
	each of the vectors in its frame (X,Y,Z) that are shown, ask
	yourself, what is the RoomSpace vector that this arrow depicts?
	Then enter that value in the appropriate X,Y, or Z column in the
	PropToTracker frame.

	4. For some of the props you may want the origin for the prop to
	be somewhere on the prop other than where the tracker is
	positioned.  For these, measure the distance from the tracker to
	where you want the origin to be, call this vector V.  Then, put
	this vector into the translation part of the PropToTracker frame.
	Be careful, the translation is applied after the rotation
	specified in the frame, so for example, if the first column of the
	frame is (0,1,0,0), you should put the Y component of V in that
	column, so it would become (0,1,0,Y[1]).  If it were (0,-1,0,0),
	you would need to put (0,-1,0,-Y[1]).
	*/
class InputDeviceVRPNTracker : public AbstractInputDevice
{
public:
#ifdef USE_VRPN
	InputDeviceVRPNTracker(
		const std::string							   &vrpnTrackerDeviceName,
		const std::vector<std::string>				   &eventsToGenerate,
		const double								   &trackerUnitsToRoomUnitsScale,
		const glm::dmat4								   &deviceToRoom,
		const std::vector<glm::dmat4>			       &propToTracker,
		const std::vector<glm::dmat4>				   &finalOffset,
		const bool									   &waitForNewReportInPoll,
		const bool									   &convertLHtoRH = false,
		const bool									   &ignoreZeroes = false);

	InputDeviceVRPNTracker(const std::string name, const ConfigMapRef map);

	virtual ~InputDeviceVRPNTracker();

	void processEvent(const glm::dmat4 &vrpnEvent, int sensorNum, const boost::posix_time::ptime &msg_time);
	std::string getEventName(int trackerNumber);
	void pollForInput(std::vector<EventRef> &events);
	void setPrintSensor0(bool b) { _printSensor0 = b; }

private:
	vrpn_Connection        *_vrpnConnection;
	vrpn_Tracker_Remote    *_vrpnDevice;
	std::vector<std::string>      _eventNames;
	double                  _trackerUnitsToRoomUnitsScale;
	glm::dmat4         _deviceToRoom;
	std::vector<glm::dmat4>  _propToTracker;
	std::vector<glm::dmat4>  _finalOffset;
	bool                    _printSensor0;
	bool                    _waitForNewReport;
	bool                    _convertLHtoRH;
	bool                    _ignoreZeroes;
	bool                    _newReportFlag;
	std::vector<EventRef>         _pendingEvents;
#else
	InputDeviceVRPNTracker(
		const std::string							   &vrpnTrackerDeviceName,
		const std::vector<std::string>			   &eventsToGenerate,
		const double								   &trackerUnitsToRoomUnitsScale,
		const glm::dmat4				   &deviceToRoom,
		const std::vector<glm::dmat4> &propToTracker,
		const std::vector<glm::dmat4> &finalOffset,
		const bool									   &waitForNewReportInPoll,
		const bool									   &convertLHtoRH = false,
		const bool									   &ignoreZeroes = false)
	{
		BOOST_ASSERT_MSG(false, "VRPN is currently unsupported. Set cmake option USE_VRPN to ON and recompile.");
	}

	InputDeviceVRPNTracker(const std::string name, const ConfigMapRef map)
	{
		BOOST_ASSERT_MSG(false, "VRPN is currently unsupported. Set cmake option USE_VRPN to ON and recompile.");
	}

	virtual ~InputDeviceVRPNTracker() {}
	void pollForInput(std::vector<EventRef> &events) {}

#endif // USE_VRPN
};

} // end namespace

#endif


