// SpaceMouseATL.cpp : Implementation of WinMain


#include "stdafx.h"
#include "resource.h"
#include "SpaceMouseATL_i.h"


using namespace ATL;


class CSpaceMouseATLModule : public ATL::CAtlExeModuleT< CSpaceMouseATLModule >
{
public :
	DECLARE_LIBID(LIBID_SpaceMouseATLLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SPACEMOUSEATL, "{12024477-AB83-4872-9D9C-80FD9AC70D91}")
	};

CSpaceMouseATLModule _AtlModule;



//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
								LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	return _AtlModule.WinMain(nShowCmd);
}

