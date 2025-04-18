#pragma once

class CPngStatic : public CWnd
{
public:
	CPngStatic();
	virtual ~CPngStatic();

	BOOL Create(HWND hParent, CRect rRect);
	bool LoadFromResource(LPCTSTR pName, LPCTSTR pType);
    void Refresh();

protected:
    void UpdateLayered();

	Gdiplus::Bitmap* m_pBitmap;
	HGLOBAL m_hBuffer;
	
private:
    CString m_strPath;
    BYTE m_alpha;
};
