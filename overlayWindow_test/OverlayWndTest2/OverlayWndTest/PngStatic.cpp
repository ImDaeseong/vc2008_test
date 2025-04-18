#include "StdAfx.h"
#include "PngStatic.h"

CPngStatic::CPngStatic()
{
	m_pBitmap = NULL;
    m_hBuffer = NULL;
}

CPngStatic::~CPngStatic()
{
	if (m_pBitmap)
    {
        delete m_pBitmap;
        m_pBitmap = NULL;
    }

    if (m_hBuffer)
    {
        ::GlobalFree(m_hBuffer);
        m_hBuffer = NULL;
    }
}

bool CPngStatic::LoadFromResource(LPCTSTR pName, LPCTSTR pType)
{
    if (m_pBitmap)
    {
        delete m_pBitmap;
        m_pBitmap = NULL;
    }

    if (m_hBuffer)
    {
        ::GlobalFree(m_hBuffer);
        m_hBuffer = NULL;
    }

    HMODULE hInst = AfxGetInstanceHandle();

    HRSRC hResource = ::FindResource(hInst, pName, pType);
    if (!hResource) return false;

    DWORD imageSize = ::SizeofResource(hInst, hResource);
    if (!imageSize) return false;

    const void* pResourceData = ::LockResource(::LoadResource(hInst, hResource));
    if (!pResourceData) return false;

    m_hBuffer = ::GlobalAlloc(GMEM_MOVEABLE, imageSize);
    if (!m_hBuffer) return false;

    void* pBuffer = ::GlobalLock(m_hBuffer);
    if (!pBuffer) return false;

    CopyMemory(pBuffer, pResourceData, imageSize);
    ::GlobalUnlock(m_hBuffer);

    IStream* pStream = NULL;
    if (::CreateStreamOnHGlobal(m_hBuffer, FALSE, &pStream) == S_OK)
    {
        m_pBitmap = Gdiplus::Bitmap::FromStream(pStream);
        pStream->Release();

        if (m_pBitmap && m_pBitmap->GetLastStatus() == Gdiplus::Ok)
            return true;

        delete m_pBitmap;
        m_pBitmap = NULL;
    }

    ::GlobalFree(m_hBuffer);
    m_hBuffer = NULL;

    return false;
}

void CPngStatic::Refresh()
{
	UpdateLayered();
}

BOOL CPngStatic::Create(HWND hParent, CRect rRect)
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
		//UpdateLayered();
    }

    return bCreate;
}

void CPngStatic::UpdateLayered()
{
    if (!m_pBitmap) return;

    CRect rcClient;
    GetClientRect(&rcClient);

    CDC* pScreenDC = GetDC();
    CDC memDC;
    memDC.CreateCompatibleDC(pScreenDC);

    BITMAPINFO bmpInfo = {};
    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biWidth = rcClient.Width();
    bmpInfo.bmiHeader.biHeight = -rcClient.Height(); // top-down DIB
    bmpInfo.bmiHeader.biPlanes = 1;
    bmpInfo.bmiHeader.biBitCount = 32;
    bmpInfo.bmiHeader.biCompression = BI_RGB;

    void* pBits = NULL;
    HBITMAP hBitmap = CreateDIBSection(memDC.m_hDC, &bmpInfo, DIB_RGB_COLORS, &pBits, NULL, 0);
    HBITMAP hOldBitmap = (HBITMAP)memDC.SelectObject(hBitmap);

    Graphics graphics(memDC);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.Clear(Color(0, 0, 0, 0));

    graphics.DrawImage(m_pBitmap, 0, 0, rcClient.Width(), rcClient.Height());

    POINT ptSrc = { 0, 0 };
    SIZE sizeWnd = { rcClient.Width(), rcClient.Height() };
    BLENDFUNCTION blend = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

    ::UpdateLayeredWindow(m_hWnd, pScreenDC->m_hDC, NULL, &sizeWnd, memDC.m_hDC, &ptSrc, 0, &blend, ULW_ALPHA);

    memDC.SelectObject(hOldBitmap);
    DeleteObject(hBitmap);
    ReleaseDC(pScreenDC);
}