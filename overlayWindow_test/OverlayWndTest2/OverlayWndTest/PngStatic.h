#pragma once

class CPngStatic : public CWnd
{
public:
	CPngStatic();
	virtual ~CPngStatic();

	BOOL Create(HWND hParent, CRect rRect);

protected:
    void UpdateLayered();

	bool LoadFromResource(LPCTSTR pName, LPCTSTR pType);
	Gdiplus::Bitmap* m_pBitmap;
	HGLOBAL m_hBuffer;
};
