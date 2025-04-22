#pragma once

class COverlayWnd : public CWnd
{
public:
	COverlayWnd();
	~COverlayWnd();

	void UpdateDrawText();

	void setDrawText(CString strText);
	void setDrawFont(CString strFont);
	void setDrawColor(Gdiplus::Color textcolor);
	void setDrawColor(COLORREF color);

protected:
    afx_msg void OnSize(UINT nType, int cx, int cy);
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

	CString m_strText;

	CString m_strFont;

	Gdiplus::Color m_textcolor;
};
