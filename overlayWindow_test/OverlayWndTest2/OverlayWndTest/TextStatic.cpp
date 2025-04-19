#include "stdafx.h"
#include "TextStatic.h"

CTextStatic::CTextStatic()
{
    m_strText = _T("");
    m_nFontSize = 12;
	m_strFontName = "돋움";
	m_fontStyle = FontStyleRegular;
    m_txtColor = RGB(0, 0, 0);
    m_bkColor = RGB(0, 0, 0);//RGB(255, 255, 0);
    m_alpha = 0;//255;
}

CTextStatic::~CTextStatic()
{
}

BOOL CTextStatic::Create(HWND hParent, CRect rRect, const CString& strText, 
		int nFontSize, CString strFontName, INT fontStyle, COLORREF txtColor)
{
    CString strClassName = AfxRegisterWndClass(0);

    BOOL bCreate = CWnd::CreateEx(
		WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT,                                  
		strClassName,                                  
		_T(""),                                  
		WS_POPUP | WS_VISIBLE,                                   
		rRect,                                  
		CWnd::FromHandle(hParent),                                  
		0);

    if (bCreate)
    {
        m_strText = strText;
    	m_nFontSize = nFontSize;
		m_strFontName = strFontName;
		m_fontStyle = fontStyle;
    	m_txtColor = txtColor;    
		UpdateLayered();
    }

    return bCreate;
}

void CTextStatic::SetText(const CString& strText)
{
    m_strText = strText;
    UpdateLayered();
}

void CTextStatic::SetFontSize(int nFontSize)
{
    m_nFontSize = nFontSize;
    UpdateLayered();
}

void CTextStatic::SetFontName(CString strFontName)
{
	m_strFontName = strFontName;
	UpdateLayered();
}

void CTextStatic::SetFontStyle(INT fontStyle)
{
	m_fontStyle = fontStyle;
	UpdateLayered();
}

void CTextStatic::SetTextColor(COLORREF txtColor)
{
    m_txtColor = txtColor;
    UpdateLayered();
}

void CTextStatic::UpdateLayered()
{
    if (!::IsWindow(m_hWnd)) return;

	USES_CONVERSION;

    CRect rClient;
    GetClientRect(&rClient);
	
    CDC* pScreenDC = GetDC();
    CDC memDC;
    if (!memDC.CreateCompatibleDC(pScreenDC))
    {
        ReleaseDC(pScreenDC);
        return;
    }
	
	// Alpha 채널 포함된 DIBSection 비트맵 생성
    BITMAPINFO bmpInfo;
    ZeroMemory(&bmpInfo, sizeof(bmpInfo));
    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biWidth = rClient.Width();
    bmpInfo.bmiHeader.biHeight = -rClient.Height();
    bmpInfo.bmiHeader.biPlanes = 1;
    bmpInfo.bmiHeader.biBitCount = 32;
    bmpInfo.bmiHeader.biCompression = BI_RGB;

    void* pBits = NULL;
    HBITMAP hBitmap = CreateDIBSection(memDC.m_hDC, &bmpInfo, DIB_RGB_COLORS, &pBits, NULL, 0);
    if (!hBitmap)
    {
        ReleaseDC(pScreenDC);
        return;
    }

    HBITMAP hOldBitmap = (HBITMAP)memDC.SelectObject(hBitmap);

    Graphics graphics(memDC.m_hDC);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	graphics.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);
    graphics.SetPixelOffsetMode(PixelOffsetModeHighQuality);

    //배경색
    Color bkColor(m_alpha, GetRValue(m_bkColor), GetGValue(m_bkColor), GetBValue(m_bkColor));
    graphics.Clear(bkColor);

    // 폰트 설정
    Gdiplus::FontFamily fontFamily(A2W(m_strFontName));
    Gdiplus::Font font(&fontFamily, (REAL)m_nFontSize, FontStyleRegular, UnitPixel);

    // 텍스트 브러시
    Gdiplus::SolidBrush textBrush(Color(255, GetRValue(m_txtColor), GetGValue(m_txtColor), GetBValue(m_txtColor)));

    // 텍스트 그리기
    RectF layoutRect(0.0f, 0.0f, (REAL)rClient.Width(), (REAL)rClient.Height());
    WCHAR wszText[256] = { 0 };
    MultiByteToWideChar(CP_ACP, 0, m_strText, -1, wszText, 255);
    graphics.DrawString(wszText, -1, &font, layoutRect, NULL, &textBrush);

    // layered window 적용
    SIZE sizeWnd = { rClient.Width(), rClient.Height() };
    POINT ptSrc = { 0, 0 };
    BLENDFUNCTION blend = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

	::UpdateLayeredWindow(m_hWnd, pScreenDC->m_hDC, NULL, &sizeWnd, memDC.m_hDC, &ptSrc, 0, &blend, ULW_ALPHA);

    // 정리
    memDC.SelectObject(hOldBitmap);
    DeleteObject(hBitmap);
    ReleaseDC(pScreenDC);
}
