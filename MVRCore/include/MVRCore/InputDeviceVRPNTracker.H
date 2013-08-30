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
		const glm::mat4								   &deviceToRoom,
		const std::vector<glm::mat4>			       &propToTracker,
		const std::vector<glm::mat4>				   &finalOffset,
		const bool									   &waitForNewReportInPoll,
		const bool									   &convertLHtoRH = false,
		const bool									   &ignoreZeroes = false);

	InputDeviceVRPNTracker(const std::string name, const ConfigMapRef map);

	virtual ~InputDeviceVRPNTracker();

	void processEvent(const glm::mat4 &vrpnEvent, int sensorNum);
	std::string getEventName(int trackerNumber);
	void pollForInput(std::vector<EventRef> &events);
	void setPrintSensor0(bool b) { _printSensor0 = b; }

private:
	vrpn_Connection        *_vrpnConnection;
	vrpn_Tracker_Remote    *_vrpnDevice;
	std::vector<std::string>      _eventNames;
	double                  _trackerUnitsToRoomUnitsScale;
	glm::mat4         _deviceToRoom;
	std::vector<glm::mat4>  _propToTracker;
	std::vector<glm::mat4>  _finalOffset;
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
		const glm::mat4				   &deviceToRoom,
		const std::vector<glm::mat4> &propToTracker,
		const std::vector<glm::mat4> &finalOffset,
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


