//========================================================================
// MinVR
// Platform:    Mac OS X
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

// OSX Specific Implementation

#ifdef USE_SPACENAV

// Include the 3D Connexion API from /Library/Frameworks
#include <3DconnexionClient/ConnexionClientAPI.h>

#include  "MVRCore/InputDeviceSpaceNav.H"
#include <boost/thread.hpp>

UInt16 gClientID;
boost::mutex  gEventBufferMutex;
static std::vector<MinVR::EventRef> gEventBuffer;

// Mark these function for weak-linking to avoid runtime issues if the
// 3dconnexion framework is not installed -- not sure if we really
// need this, since we are statically linking.
//
extern OSErr InstallConnexionHandlers(ConnexionMessageHandlerProc messageHandler,
	ConnexionAddedHandlerProc addedHandler,
	ConnexionRemovedHandlerProc removedHandler)__attribute__((weak_import));

namespace MinVR {


class TdxDeviceEvents
{
public:
	TdxDeviceEvents();
	virtual ~TdxDeviceEvents();

	static OSStatus threadProc(void *param);

private:
	EventQueueRef m_mainEventQueue;
};


void messageHandlerProc(io_connect_t connection, natural_t messageType,	void *messageArgument)
{


	ConnexionDeviceStatePtr state;
	state = (ConnexionDeviceStatePtr)messageArgument;
	static bool sPushedDown = false;
	static bool sTwistRight = false;
	static bool sTwistLeft = false;
	static bool sTimedTwistRight = false;
	static bool sTimedTwistLeft = false;

	switch(messageType)
	{
	case kConnexionMsgDeviceState:
		/* Device state messages are broadcast to all clients.  It is up to
		* the client to figure out if the message is meant for them. This
		* is done by comparing the "client" id sent in the message to our
		* assigned id when the connection to the driver was established.
		*/
		if (state->client == gClientID)
		{
			/* report = the kind of device data that's being reported */
			switch (state->command)
			{
			case kConnexionCmdHandleAxis:
				{
					static SInt16 zerodata[] = {0, 0, 0, 0, 0, 0};
					static Boolean isZero = FALSE;
					static Boolean wasZero = FALSE;

					if (isZero = (memcmp(state->axis, zerodata, sizeof(zerodata)) == 0)) {
						glm::dvec3 trans(0.0);
						glm::dvec3 rot(0.0);
							
						gEventBufferMutex.lock();
						gEventBuffer.push_back(EventRef(new MinVR::Event("SpaceNav_Trans", trans)));
						gEventBuffer.push_back(EventRef(new MinVR::Event("SpaceNav_Rot", rot)));
						gEventBufferMutex.unlock();
					}
					else {
						glm::dvec3 trans(state->axis[0], state->axis[1], state->axis[2]);
						glm::dvec3 rot(state->axis[3], state->axis[4], state->axis[5]);
						//cout << "Trans: " << trans << endl;
						//cout << "Rot:   " << rot << endl;

						gEventBufferMutex.lock();
						gEventBuffer.push_back(EventRef(new MinVR::Event("SpaceNav_Trans", trans)));
						gEventBuffer.push_back(EventRef(new MinVR::Event("SpaceNav_Rot", rot)));
						gEventBufferMutex.unlock();
					}
					break;
				}

			case kConnexionCmdAppSpecific:
				break;

			case kConnexionCmdHandleButtons:
				{
					static bool buttonPressed[] = {false, false};
					if ((state->buttons & 1) && (!buttonPressed[0])) {
						//cout << "btn 1 down" << endl;
						gEventBufferMutex.lock();
						gEventBuffer.push_back(EventRef(new MinVR::Event("SpaceNav_Btn1_down")));
						gEventBufferMutex.unlock();
						buttonPressed[0] = true;
					}
					else if ((!(state->buttons & 1)) && (buttonPressed[0])) {
						//cout << "btn 1 up" << endl;
						gEventBufferMutex.lock();
						gEventBuffer.push_back(EventRef(new MinVR::Event("SpaceNav_Btn1_up")));
						gEventBufferMutex.unlock();
						buttonPressed[0] = false;
					}
					if ((state->buttons & 2) && (!buttonPressed[1])) {
						//cout << "btn 2 down" << endl;
						gEventBufferMutex.lock();
						gEventBuffer.push_back(EventRef(new MinVR::Event("SpaceNav_Btn2_down")));
						gEventBufferMutex.unlock();
						buttonPressed[1] = true;
					}
					else if ((!(state->buttons & 2)) && (buttonPressed[1])) {
						//cout << "btn 2 up" << endl;
						gEventBufferMutex.lock();
						gEventBuffer.push_back(EventRef(new MinVR::Event("SpaceNav_Btn2_up")));
						gEventBufferMutex.unlock();
						buttonPressed[1] = false;
					}
					break;
				}
			}
		}
		break;

	default:
		// other messageTypes can happen and should be ignored
		break;
	}
}


TdxDeviceEvents::TdxDeviceEvents()
{
	// Check if the driver is installed
	if (InstallConnexionHandlers == NULL)
		return;

	/* must save the main event queue while we're running within the main thread.
	* GetMainEventQueue() is not thread-safe and will cause big problems if
	* called from the spawned thread
	*/
	m_mainEventQueue = GetMainEventQueue();

	// Create the listening thread for device events
	MPCreateTask(&threadProc, 0, 512000, NULL, NULL, NULL, 0, NULL);
}


TdxDeviceEvents::~TdxDeviceEvents()
{
	// Check if the driver is installed
	if (InstallConnexionHandlers == NULL)
		return;

	if (gClientID)
		UnregisterConnexionClient(gClientID);
	CleanupConnexionHandlers();
}


OSStatus TdxDeviceEvents::threadProc(void* param)
{
	if (InstallConnexionHandlers(messageHandlerProc, 0L, 0L) == noErr)  {
		gClientID = RegisterConnexionClient(kConnexionClientWildcard,
			NULL,
			kConnexionClientModeTakeOver,
			kConnexionMaskAll);
		SInt32 result = 0;
		//cout << "Calibrating space nav" << endl;
		ConnexionControl(kConnexionCtlCalibrate, 0, &result);
		//cout << "Calibrated" << endl;
		RunCurrentEventLoop(kEventDurationForever);
	}
	else
	{ cerr << "Error initializing space navigator device." << endl;
	}

	return noErr;
}


void InputDeviceSpaceNav::setup()
{
	new TdxDeviceEvents();
}


void InputDeviceSpaceNav::pollForInput(std::vector<MinVR::EventRef> &events)
{
	gEventBufferMutex.lock();
	events.push_back(gEventBuffer);
	gEventBuffer.clear();
	gEventBufferMutex.unlock();
}


} // end namespace

#endif // use spacenav
