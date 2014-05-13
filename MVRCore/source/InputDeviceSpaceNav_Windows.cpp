//========================================================================
// MinVR
// Platform:    Windows
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

// Windows Specific Implementation

#ifdef USE_SPACENAV

#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
#include <atltypes.h>
#include <atlctl.h>
#include <atlhost.h>
using namespace ATL;

#include "MVRCore/InputDeviceSpaceNav.h"

#import "progid:TDxInput.Device.1" no_namespace

CComPtr<ISensor> g3DSensor;
MSG Message;

#include <atlstr.h>

LPTSTR SPACENAV(_T("SpaceWareMessage00"));
UINT SN_MESS;


namespace MinVR {

InputDeviceSpaceNav::~InputDeviceSpaceNav() {

}

void InputDeviceSpaceNav::setup()
{
	HRESULT hr;
	//cout << "Win32 Setup";
	SN_MESS = RegisterWindowMessage(SPACENAV);
	CComPtr<IUnknown> _3DxDevice;


	hr=::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED );
	if (!SUCCEEDED(hr))	{
		CString strError;
		strError.FormatMessage (_T("Error 0x%x"), hr);
		::MessageBox (NULL, strError, _T("CoInitializeEx failed"), MB_ICONERROR|MB_OK);

	}


	// Create the device object
	hr = _3DxDevice.CoCreateInstance(__uuidof(Device));
	if (SUCCEEDED(hr)) {
		CComPtr<ISimpleDevice> _3DxSimpleDevice;

		hr = _3DxDevice.QueryInterface(&_3DxSimpleDevice);
		if (SUCCEEDED(hr)) {
			// Get the interfaces to the sensor and the keyboard;
			g3DSensor = _3DxSimpleDevice->Sensor;

			// Associate a configuration with this device
			//_3DxSimpleDevice->LoadPreferences(_T("Cube3DPolling"));
			// Connect to the driver
			_3DxSimpleDevice->Connect();

			// Create timer used to poll the 3dconnexion device
			//gTimerId = ::SetTimer(NULL, 0, 25, _3DxTimerProc);
		}
	}
}


void InputDeviceSpaceNav::pollForInput(std::vector<EventRef> &events)
{
	if(PeekMessage(&Message, 0, SN_MESS, SN_MESS, PM_REMOVE)){
		DispatchMessage(&Message);
		if (g3DSensor) {
			try {
				CComPtr<IAngleAxis> pRotation = g3DSensor->Rotation;
				CComPtr<IVector3D> pTranslation = g3DSensor->Translation;

				if (pRotation->Angle > 0. || pTranslation->Length > 0.)	{
					//calibrating the same as mac, windows results are (-1, 1) for rot
					// and (-80, 80) for trans, mac are not so simple, also mac have some axis reversed, possibly
					// settings stuff?
					glm::dvec3 trans(pTranslation->X / 80.0, pTranslation->Z / 80.0, -pTranslation->Y / 80.0);
					glm::dvec3 rot(pRotation->X, pRotation->Z, -pRotation->Y);

					events.push_back(EventRef(new Event("SpaceNav_Trans", trans)));
					events.push_back(EventRef(new Event("SpaceNav_Rot", rot)));
					//cout << trans << " " << rot << endl;
				}
				else {
					glm::dvec3 trans(0.0);
					glm::dvec3 rot(0.0);


					events.push_back(EventRef(new Event("SpaceNav_Trans", trans)));
					events.push_back(EventRef(new Event("SpaceNav_Rot", rot)));
				}

			}
			catch (...)	{
			}
		}
	}
}

} // end namespace
#endif // use_spacenav