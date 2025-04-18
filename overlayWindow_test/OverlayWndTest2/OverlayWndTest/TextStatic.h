#pragma once

class CTextStatic : public CWnd
{
public:
    CTextStatic();
    virtual ~CTextStatic();

    BOOL Create(HWND hParent, CRect rRect, const CString& strText, 
		int nFontSize, CString strFontName, 
		INT fontStyle, COLORREF txtColor);

    void SetText(const CString& strText);
    void SetFontSize(int nFontSize);
	void SetFontName(CString strFontName);
	void SetFontStyle(INT fontStyle);
    void SetTextColor(COLORREF txtColor);

protected:
    void UpdateLayered();

private:
    CString     m_strText;
    int         m_nFontSize;
	CString     m_strFontName;
	INT         m_fontStyle;
    COLORREF    m_txtColor;
    COLORREF    m_bkColor;
    BYTE        m_alpha;
};
