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

#ifndef MINVR_TUIOEVENTS_H
#define MINVR_TUIOEVENTS_H

#include  <G3DLite/G3DLite.h>

#ifdef USE_TUIO
#include  <TuioClient.h>
#endif


#include "MVRCore/AbstractInputDevice.h"
#include "MVRCore/ConfigMap.H"
#include "MVRCore/Event.H"



#define TUIO_PORT  3333

namespace MinVR {

class InputDeviceTUIOClient : public AbstractInputDevice
{
public:

#ifdef USE_TUIO
	InputDeviceTUIOClient(int port = TUIO_PORT, double  xScaleFactor = 1.0, double  yScaleFactor=1.0 );
	InputDeviceTUIOClient( const std::string name, G3DLite::Log *log, const ConfigMapRef map );
	virtual ~InputDeviceTUIOClient();
	
	void pollForInput( G3DLite::Array<EventRef>  &events );


private:
	TUIO::TuioClient *_tuioClient;
	G3DLite::Set<int>    _cursorsDown;
	double      _xScale;
	double      _yScale;

#else
	InputDeviceTUIOClient(int port = TUIO_PORT, double  xScaleFactor = 1.0, double  yScaleFactor=1.0 )
	{
		alwaysAssertM(false, "TUIO is currently unsupported. Set cmake option USE_TUIO to ON and recompile.");
	}

	InputDeviceTUIOClient( const std::string name, G3DLite::Log *log, const ConfigMapRef map )
	{
		alwaysAssertM(false, "TUIO is currently unsupported. Set cmake option USE_TUIO to ON and recompile.");
	}

	virtual ~InputDeviceTUIOClient() {}
	void pollForInput( G3DLite::Array<EventRef> &events) {}


#endif                   // USE_TUIO
};

}                        // end namespace

#endif                  // VRG3D_TUIOEVENTS_H
