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

//#define _ATL_ATTRIBUTES 1
//
//#define _ATL_APARTMENT_THREADED
//#define _ATL_NO_AUTOMATIC_NAMESPACE
//
//#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit
//
//#define _WIN32_DCOM
//
//#include <objbase.h>
//#include <atlbase.h>
//#include <atlcom.h>
//#include <atlwin.h>
//#include <atltypes.h>
//#include <atlctl.h>
//#include <atlhost.h>
//
//#include "manip_spaceMouse.h"

#include "stdafx.h"
using namespace ATL;
using namespace osgVisual;

#include "..\vJoyFeederSDK\inc\vjoyinterface.h"
#include "..\vJoyFeederSDK\inc\public.h"

#include "C:\Users\gbrill\Source\Repos\SpaceMouse\ConsoleATLTest\Debug\tdxinput.tlh"

bool initFeeder(UINT iInterface = 1);
void setButtonData(int button, bool state, UINT iInterface = 1);
void setJoyData(long X, long Y, long Z, long XR, long ZR, UINT iInterface = 1);

//#import "progid:TDxInput.Device" no_namespace


static _ATL_FUNC_INFO KeyDownInfo = { CC_STDCALL, VT_EMPTY, 1, { VT_INT } };
static _ATL_FUNC_INFO KeyUpInfo = { CC_STDCALL, VT_EMPTY, 1, { VT_INT } };

extern char buttonInfo[512];


class CKeyboardEvents :
	public IDispEventSimpleImpl<1, CKeyboardEvents, &__uuidof(_IKeyboardEvents)>
{
public:

	HRESULT __stdcall KeyDownEvent(int v)
	{

		sprintf(buttonInfo, "Button Down:%d", v);

		setButtonData(v, TRUE, JOYSTICK_ID);


		//PlaySound((LPCTSTR)SND_ALIAS_SYSTEMEXCLAMATION, 0, SND_ALIAS_ID );
		return S_OK;

	}

	HRESULT __stdcall KeyUpEvent(int v)
	{
		sprintf(buttonInfo, "Button Up:%d", v);
		setButtonData(v, FALSE, JOYSTICK_ID);
		//PlaySound("MailBeep", 0, SND_SYSTEM);
		return S_OK;
	}



	BEGIN_SINK_MAP(CKeyboardEvents)
		SINK_ENTRY_INFO(1, __uuidof(_IKeyboardEvents), 1, KeyDownEvent, &KeyDownInfo)
		SINK_ENTRY_INFO(1, __uuidof(_IKeyboardEvents), 2, KeyUpEvent, &KeyUpInfo)
	END_SINK_MAP()
};

class SpaceMouse::SpaceMouseImpl
{
public:
	CKeyboardEvents events;
	CComPtr<IKeyboard>  m_DevKeyb;

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

				initKeyBoardEvents(); //Greg
				return(1); //Base::SUCCESS
			}
		}

		_supported = NOT_SUPPORTED;

		return(0);	//Base::FAILURE
	}

	void shutdown()
	{
		HRESULT hrk = events.DispEventUnadvise(m_DevKeyb);

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




	HRESULT initKeyBoardEvents()
	{
	//	CComPtr<IVector3D> pTranslation = _3DKeyboard->
			
		HRESULT hrk = _3DxDevice->get_Keyboard(&m_DevKeyb);


		if (SUCCEEDED(hrk) && m_DevKeyb.p)
		{
			
			hrk = events.DispEventAdvise(m_DevKeyb);

			if (FAILED(hrk))
			{

			//	cerr << "3Dconnexion plugin [" << hrk << "]: " << "FAILED to register for device button events.\n";
			}
		}
		return S_OK;
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

int SpaceMouse::vJoyInit(int device=1)
{


	USHORT X, Y, Z, ZR, XR;							// Position of several axes
	BYTE id = 1;										// ID of the target vjoy device (Default is 1)
	UINT iInterface = device;								// Default target vJoy device
	BOOL ContinuousPOV = FALSE;						// Continuous POV hat (or 4-direction POV Hat)
	int count = 0;


	// Get the ID of the target vJoy device
//	if (argc>1 && wcslen(argv[1]))
//		sscanf_s((char *)(argv[1]), "%d", &iInterface);


	// Get the driver attributes (Vendor ID, Product ID, Version Number)
	if (!vJoyEnabled())
	{
		_tprintf("vJoy driver not enabled: Failed Getting vJoy attributes.\n");
		return -2;
	}
	else
	{
		_tprintf("Vendor: %S\nProduct :%S\nVersion Number:%S\n", TEXT(GetvJoyManufacturerString()), TEXT(GetvJoyProductString()), TEXT(GetvJoySerialNumberString()));
	};

	WORD VerDll, VerDrv;
	if (!DriverMatch(&VerDll, &VerDrv))
	{
		_tprintf("Failed\r\nvJoy Driver (version %04x) does not match vJoyInterface DLL (version %04x)\n", VerDrv, VerDll);
	}
	else
		_tprintf("OK - vJoy Driver and vJoyInterface DLL match vJoyInterface DLL (version %04x)\n", VerDrv);

	// Get the state of the requested device
	VjdStat status = GetVJDStatus(iInterface);
	switch (status)
	{
	case VJD_STAT_OWN:
		_tprintf("vJoy Device %d is already owned by this feeder\n", iInterface);
		break;
	case VJD_STAT_FREE:
		_tprintf("vJoy Device %d is free\n", iInterface);
		break;
	case VJD_STAT_BUSY:
		_tprintf("vJoy Device %d is already owned by another feeder\nCannot continue\n", iInterface);
		return -3;
	case VJD_STAT_MISS:
		_tprintf("vJoy Device %d is not installed or disabled\nCannot continue\n", iInterface);
		return -4;
	default:
		_tprintf("vJoy Device %d general error\nCannot continue\n", iInterface);
		return -1;
	};

	BOOL AxisX = GetVJDAxisExist(iInterface, HID_USAGE_X);
	BOOL AxisY = GetVJDAxisExist(iInterface, HID_USAGE_Y);
	BOOL AxisZ = GetVJDAxisExist(iInterface, HID_USAGE_Z);
	BOOL AxisRX = GetVJDAxisExist(iInterface, HID_USAGE_RX);
	BOOL AxisRZ = GetVJDAxisExist(iInterface, HID_USAGE_RZ);
	// Get the number of buttons and POV Hat switchessupported by this vJoy device
	int nButtons = GetVJDButtonNumber(iInterface);
	int ContPovNumber = GetVJDContPovNumber(iInterface);
	int DiscPovNumber = GetVJDDiscPovNumber(iInterface);

	// Print results
	//_tprintf("\nvJoy Device %d capabilities:\n", iInterface);
	//_tprintf("Numner of buttons\t\t%d\n", nButtons);
	//_tprintf("Numner of Continuous POVs\t%d\n", ContPovNumber);
	//_tprintf("Numner of Descrete POVs\t\t%d\n", DiscPovNumber);
	//_tprintf("Axis X\t\t%s\n", AxisX ? "Yes" : "No");
	//_tprintf("Axis Y\t\t%s\n", AxisX ? "Yes" : "No");
	//_tprintf("Axis Z\t\t%s\n", AxisX ? "Yes" : "No");
	//_tprintf("Axis Rx\t\t%s\n", AxisRX ? "Yes" : "No");
	//_tprintf("Axis Rz\t\t%s\n", AxisRZ ? "Yes" : "No");



	// Acquire the target
	if ((status == VJD_STAT_OWN) || ((status == VJD_STAT_FREE) && (!AcquireVJD(iInterface))))
	{
		_tprintf("Failed to acquire vJoy device number %d.\n", iInterface);
		return -1;
	}
	else
	{
		_tprintf("Acquired: vJoy device number %d.\n", iInterface);
	}

	return true;




}

//void SpaceMouse::setJoyData(long X, long Y, long Z, long XR, long ZR)
//{
//	int count, res;
//
//	res = SetAxis(X, iInterface, HID_USAGE_X);
//	res = SetAxis(Y, iInterface, HID_USAGE_Y);
//	res = SetAxis(Z, iInterface, HID_USAGE_Z);
//	res = SetAxis(XR, iInterface, HID_USAGE_RX);
//	res = SetAxis(ZR, iInterface, HID_USAGE_RZ);
//
//	// Press/Release Buttons
//
//
//}