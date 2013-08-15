//========================================================================
// MinVR
// Platform:    Windows
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

using namespace G3DLite;

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


void InputDeviceSpaceNav::pollForInput(Array<EventRef> &events)
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
					Vector3 trans(pTranslation->X / 80.0, pTranslation->Z / 80.0, -pTranslation->Y / 80.0);
					Vector3 rot(pRotation->X, pRotation->Z, -pRotation->Y);

					events.append(new Event("SpaceNav_Trans", trans));
					events.append(new Event("SpaceNav_Rot", rot));
					//cout << trans << " " << rot << endl;
				}
				else {
					Vector3 trans = Vector3::zero();
					Vector3 rot = Vector3::zero();


					events.append(new Event("SpaceNav_Trans", trans));
					events.append(new Event("SpaceNav_Rot", rot));
				}

			}
			catch (...)	{
			}
		}
	}
}

} // end namespace
#endif // use_spacenav