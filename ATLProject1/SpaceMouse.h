// SpaceMouse.h : Declaration of the SpaceMouse class

#pragma once

#include <atlhandlerimpl.h>

using namespace ATL;

class SpaceMouse : public CAtlDocumentImpl
{
public:
	SpaceMouse(void)
	{
	}

	virtual ~SpaceMouse(void)
	{
	}

	virtual HRESULT LoadFromStream(IStream* pStream, DWORD grfMode);
	virtual void InitializeSearchContent();

protected:
	void SetSearchContent(CString& value);
	virtual void OnDrawThumbnail(HDC hDrawDC, LPRECT lprcBounds);
};
