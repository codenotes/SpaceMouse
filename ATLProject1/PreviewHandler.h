// PreviewHandler.h : Declaration of the preview handler

#pragma once
#include "resource.h"       // main symbols

#define AFX_PREVIEW_STANDALONE
#include <atlhandler.h>
#include <atlhandlerimpl.h>
#include "SpaceMouse.h"
#include <atlpreviewctrlimpl.h>

#include "ATLProject1_i.h"

using namespace ATL;

// CPreviewCtrl implementation
class CPreviewCtrl : public CAtlPreviewCtrlImpl
{
protected:
	virtual void DoPaint(HDC hdc)
	{
		// you can obtain a pointer to IDocument as follows
		// CMyDoc* pDoc = (CMyDoc*)m_pDocument;
		CString strData = _T("Draw Rich Preview content here.");
		TextOut(hdc, 10, 20, strData, strData.GetLength());
	}
};

// CPreviewHandler

class ATL_NO_VTABLE CPreviewHandler :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CPreviewHandler, &CLSID_Preview>,
	public CPreviewHandlerImpl <CPreviewHandler>
{
public:
	CPreviewHandler()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_PREVIEW_HANDLER)
DECLARE_NOT_AGGREGATABLE(CPreviewHandler)

BEGIN_COM_MAP(CPreviewHandler)
	COM_INTERFACE_ENTRY(IObjectWithSite)
	COM_INTERFACE_ENTRY(IOleWindow)
	COM_INTERFACE_ENTRY(IInitializeWithStream)
	COM_INTERFACE_ENTRY(IPreviewHandler)
	COM_INTERFACE_ENTRY(IPreviewHandlerVisuals)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
		CPreviewHandlerImpl<CPreviewHandler>::FinalRelease();
	}

protected:
	virtual IPreviewCtrl* CreatePreviewControl()
	{
		// This class is defined at the beginning of this header
		CPreviewCtrl *pPreviewCtrl = NULL;
		ATLTRY(pPreviewCtrl = new CPreviewCtrl());
		return pPreviewCtrl;
	}

	virtual IDocument* CreateDocument()
	{
		SpaceMouse *pDocument = NULL;
		ATLTRY(pDocument = new SpaceMouse());
		return pDocument;
	}

};

OBJECT_ENTRY_AUTO(__uuidof(Preview), CPreviewHandler)
