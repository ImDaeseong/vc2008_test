#pragma once

class COverlayWnd : public CWnd
{
public:
	COverlayWnd();
	~COverlayWnd();

	void UpdateDrawText();

	void setDrawText(CString strText1, CString strText2, CString strText3);
	void setDrawFont(CString strFont1, CString strFont2, CString strFont3);
	void setDrawColor(Gdiplus::Color textcolor1, Gdiplus::Color textcolor2, Gdiplus::Color textcolor3);
	void setDrawColor(COLORREF color1, COLORREF color2, COLORREF color3);

protected:
    afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
    DECLARE_MESSAGE_MAP()

private:
	void DrawText(Graphics& graphics,
		CString strText,         
		CString strFontName,      
		REAL fontSize,            
		INT fontStyle,           
		RectF fRect,         
		StringAlignment alignment,
		Color textcolor,
		Gdiplus::Color bgcolor);

	CString m_strText1;
	CString m_strText2;
	CString m_strText3;

	CString m_strFont1;
	CString m_strFont2;
	CString m_strFont3;

	Gdiplus::Color m_textcolor1;
	Gdiplus::Color m_textcolor2;
	Gdiplus::Color m_textcolor3;
};
