/* -*-c++-*- osgVisual - Copyright (C) 2009-2011 Torben Dannhauer
*
* This library is based on OpenSceneGraph, open source and may be redistributed and/or modified under
* the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or
* (at your option) any later version.  The full license is in LICENSE file
* included with this distribution, and on the openscenegraph.org website.
*
* osgVisual requires for some proprietary modules a license from the correspondig manufacturer.
* You have to aquire licenses for all used proprietary modules.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* OpenSceneGraph Public License for more details.
*/

#include "manip_spaceMouse.h"


#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#define _WIN32_DCOM

#include <objbase.h>
#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
#include <atltypes.h>
#include <atlctl.h>
#include <atlhost.h>

using namespace ATL;
using namespace osgVisual;


#import "progid:TDxInput.Device" no_namespace

class SpaceMouse::SpaceMouseImpl
{
public:
	enum SupportedStatus
	{
		DETECTION_PENDING,
		SUPPORTED,
		NOT_SUPPORTED
	};

	SpaceMouseImpl() : _supported(DETECTION_PENDING)
	{
	}

	SupportedStatus isSupported() const
	{
		return(_supported);
	}

	int initialize()
	{
		HRESULT hr = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);

		// Create the device object
		hr = _3DxDevice.CoCreateInstance(__uuidof(Device));
		if (SUCCEEDED(hr))
		{
			CComPtr<ISimpleDevice> _3DxSimpleDevice;

			hr = _3DxDevice.QueryInterface(&_3DxSimpleDevice);
			if (SUCCEEDED(hr))
			{
				// Get the interfaces to the sensor and the keyboard;
				_3DSensor = _3DxSimpleDevice->Sensor;
				_3DKeyboard = _3DxSimpleDevice->Keyboard;

				// Associate a configuration with this device
				hr = _3DxSimpleDevice->LoadPreferences(_T("Animation"));
				// Connect to the driver
				hr = _3DxSimpleDevice->Connect();

				_supported = SUPPORTED;

				return(1); //Base::SUCCESS
			}
		}

		_supported = NOT_SUPPORTED;

		return(0);	//Base::FAILURE
	}

	void shutdown()
	{
		if (_3DSensor)
			_3DSensor.Release();

		if (_3DKeyboard)
			_3DKeyboard.Release();

		if (_3DxDevice)
		{
			_3DxDevice->Disconnect();
			_3DxDevice.Release();
		}
	}


	int getTranslations(double& dTX, double& dTY, double& dTZ)
	{
		try {
			CComPtr<IVector3D> pTranslation = _3DSensor->Translation;

			double dTransFactor = pTranslation->Length;
			dTransFactor /= 5.0;

			dTransFactor = 1;

			dTX = pTranslation->X * dTransFactor;
			dTY = pTranslation->Y * dTransFactor;
			dTZ = pTranslation->Z * dTransFactor;

			pTranslation.Release();
		}
		catch (...)
		{
			return 0;	//Base::FAILURE
		}

		return 1;	//Base::SUCCESS
	}

	int getRotations(double& dRX, double& dRY, double& dRZ)
	{
		try {

			IAngleAxis * ia;

			double test = _3DSensor->Rotation->X;

			CComPtr<IAngleAxis> pRotation = _3DSensor->Rotation;

			//ia = _3DSensor.QueryInterface<uuid(1EF2BAFF - 54E9 - 4706 - 9F61 - 078F7134FD35)>
			
			double dRotAngle = pRotation->Angle;
			dRotAngle /= 4000.0;

			dRX = pRotation->X * dRotAngle;
			dRY = pRotation->Y * dRotAngle;
			dRZ = pRotation->Z * dRotAngle;

			pRotation.Release();
		}
		catch (...)
		{
			return 0;	//Base::FAILURE
		}

		return 1;	//Base::SUCCESS
	}

private:
	SupportedStatus _supported;

	CComPtr<ISimpleDevice> _3DxDevice;
	CComPtr<ISensor>       _3DSensor;
	CComPtr<IKeyboard>     _3DKeyboard;
};








SpaceMouse::SpaceMouse() : _spaceMouseImpl(new SpaceMouseImpl)
{
}

SpaceMouse::~SpaceMouse()
{
	delete _spaceMouseImpl;
}

int SpaceMouse::initialize()
{
	return(_spaceMouseImpl->initialize());
}

void SpaceMouse::shutdown()
{
	_spaceMouseImpl->shutdown();
}

int SpaceMouse::getTranslations(double& dX, double& dY, double& dZ)
{
	dX = 0;
	dY = 0;
	dZ = 0;
	return(_spaceMouseImpl->getTranslations(dX, dY, dZ));
}

int SpaceMouse::getRotations(double& dX, double& dY, double& dZ)
{
	dX = 0;
	dY = 0;
	dZ = 0;
	return(_spaceMouseImpl->getRotations(dX, dY, dZ));
}