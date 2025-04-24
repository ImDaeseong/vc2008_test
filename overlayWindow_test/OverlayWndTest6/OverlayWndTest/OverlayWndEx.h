#pragma once

class COverlayWndEx : public CWnd
{
public:
	COverlayWndEx();
	~COverlayWndEx();

	void UpdateDrawText();

	void setDrawText(CString strText1, CString strText2);
	void setDrawFont(CString strFont1, CString strFont2);
	void setDrawColor(Gdiplus::Color textcolor1, Gdiplus::Color textcolor2);
	void setDrawColor(COLORREF color1, COLORREF color2);
	void setDrawBg();

	void StartLineAnimation();
    void StopLineAnimation();
	
protected:
    afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
    DECLARE_MESSAGE_MAP()

private:
	void DrawText(Graphics& graphics,
		CString strText,         
		CString strFontName,      
		REAL fontSize,            
		INT fontStyle,           
		RectF fRect, 
		Color textcolor,
		Gdiplus::Color bgcolor);

	CString m_strText1;
	CString m_strText2;

	CString m_strFont1;
	CString m_strFont2;

	Gdiplus::Color m_textcolor1;
	Gdiplus::Color m_textcolor2;

	
	int m_nLength;
    bool m_bDrawing;
	
private:
	bool LoadFromResource(LPCTSTR pName, LPCTSTR pType);
	Gdiplus::Bitmap* m_pBitmap;
	HGLOBAL m_hBuffer;
};
