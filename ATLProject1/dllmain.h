// dllmain.h : Declaration of module class.

class CATLProject1Module : public ATL::CAtlDllModuleT< CATLProject1Module >
{
public :
	DECLARE_LIBID(LIBID_ATLProject1Lib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ATLPROJECT1, "{DCC48BBE-C88B-4183-9722-D5E1E891D02F}")
};

extern class CATLProject1Module _AtlModule;
