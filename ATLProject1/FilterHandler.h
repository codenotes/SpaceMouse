// FilterHandler.h : Declaration of the filter handler

#pragma once
#include "resource.h"       // main symbols

#define AFX_PREVIEW_STANDALONE
#include <atlhandler.h>
#include <atlhandlerimpl.h>
#include "SpaceMouse.h"

#include "ATLProject1_i.h"

using namespace ATL;

// CFilterHandler

class ATL_NO_VTABLE CFilterHandler :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CFilterHandler, &CLSID_Search>,
	public CSearchFilterImpl
{
public:
	CFilterHandler()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_FILTER_HANDLER)
DECLARE_NOT_AGGREGATABLE(CFilterHandler)

BEGIN_COM_MAP(CFilterHandler)
	COM_INTERFACE_ENTRY(IPersistStream)
	COM_INTERFACE_ENTRY(IPersistFile)
	COM_INTERFACE_ENTRY(IFilter)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
		CSearchFilterImpl::FinalRelease();
	}

public:
	// IPersistStream implementation
	IFACEMETHODIMP GetClassID(CLSID* pClassID)
	{
		*pClassID = CLSID_Search;
		return S_OK;
	}

protected:
	virtual IDocument* CreateDocument()
	{
		SpaceMouse *pDocument = NULL;
		ATLTRY(pDocument = new SpaceMouse());
		return pDocument;
	}
};

OBJECT_ENTRY_AUTO(__uuidof(Search), CFilterHandler)
