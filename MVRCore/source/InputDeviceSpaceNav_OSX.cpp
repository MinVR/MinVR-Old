//========================================================================
// MinVR
// Platform:    OS X
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
						glm::vec3 trans(0.0);
						glm::vec3 rot(0.0);
							
						gEventBufferMutex.lock();
						gEventBuffer.push_back(EventRef(new MinVR::Event("SpaceNav_Trans", trans)));
						gEventBuffer.push_back(EventRef(new MinVR::Event("SpaceNav_Rot", rot)));
						gEventBufferMutex.unlock();
					}
					else {
						glm::vec3 trans(state->axis[0], state->axis[1], state->axis[2]);
						glm::vec3 rot(state->axis[3], state->axis[4], state->axis[5]);
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
