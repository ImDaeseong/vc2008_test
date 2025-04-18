#include "StdAfx.h"
#include "ButtonEx.h"

CButtonEx::CButtonEx()
{
	m_clickHandler = NULL;
}

CButtonEx::~CButtonEx()
{
}

BEGIN_MESSAGE_MAP(CButtonEx, CWnd)
    ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

BOOL CButtonEx::Create(HWND hParent, CRect rRect, const CString& strText)
{
	CString strClassName = AfxRegisterWndClass(0);

    BOOL bCreate = CWnd::CreateEx(
		WS_EX_LAYERED | WS_EX_TOPMOST,                                 
		strClassName,                                  
		_T(""),                                  
		WS_POPUP | WS_VISIBLE,                                   
		rRect,                                  
		CWnd::FromHandle(hParent),                                  
		0);

    if (bCreate)
    {    
		m_strText = strText;
		UpdateLayered();
    }

    return bCreate;
}

void CButtonEx::SetText(const CString& strText)
{
    m_strText = strText;
    UpdateLayered();
}

void CButtonEx::SetOnClick(void (*handler)())
{
    m_clickHandler = handler;
}

void CButtonEx::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_clickHandler != NULL)
        m_clickHandler();

    CWnd::OnLButtonUp(nFlags, point);
}

void CButtonEx::UpdateLayered()
{
    CRect rcClient;
    GetClientRect(&rcClient);

    CDC* pScreenDC = GetDC();
    CDC memDC;
    memDC.CreateCompatibleDC(pScreenDC);

    BITMAPINFO bmpInfo;
    ZeroMemory(&bmpInfo, sizeof(bmpInfo));
    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biWidth = rcClient.Width();
    bmpInfo.bmiHeader.biHeight = -rcClient.Height();
    bmpInfo.bmiHeader.biPlanes = 1;
    bmpInfo.bmiHeader.biBitCount = 32;
    bmpInfo.bmiHeader.biCompression = BI_RGB;

    void* pBits = NULL;
    HBITMAP hBitmap = CreateDIBSection(memDC.m_hDC, &bmpInfo, DIB_RGB_COLORS, &pBits, NULL, 0);
    HBITMAP hOldBitmap = (HBITMAP)memDC.SelectObject(hBitmap);

    Graphics graphics(memDC);
    graphics.Clear(Color(100, 30, 30, 30));

    Gdiplus::FontFamily fontFamily(L"µ¸¿ò");
    Gdiplus::Font font(&fontFamily, 20, FontStyleBold, UnitPixel);
    Gdiplus::SolidBrush brush(Color(255, 255, 255, 255));

    RectF layout(0, 0, (REAL)rcClient.Width(), (REAL)rcClient.Height());
	WCHAR wszText[256] = { 0 };
    MultiByteToWideChar(CP_ACP, 0, m_strText, -1, wszText, 255);
    graphics.DrawString(wszText, -1, &font, layout, NULL, &brush);

    POINT ptSrc = { 0, 0 };
    SIZE sizeWnd = { rcClient.Width(), rcClient.Height() };
    BLENDFUNCTION blend = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

    ::UpdateLayeredWindow(m_hWnd, pScreenDC->m_hDC, NULL, &sizeWnd, memDC.m_hDC, &ptSrc, 0, &blend, ULW_ALPHA);

    memDC.SelectObject(hOldBitmap);
    DeleteObject(hBitmap);
    ReleaseDC(pScreenDC);
}