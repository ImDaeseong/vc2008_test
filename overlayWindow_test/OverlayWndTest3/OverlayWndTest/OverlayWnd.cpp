#include "StdAfx.h"
#include "OverlayWnd.h"
#include "resource.h"

COverlayWnd::COverlayWnd()
{
	m_strText = "";

	m_strFont = _T("돋움");

	m_textcolor = Color(255, 0, 0, 0);

	m_pBitmap = NULL;
    m_hBuffer = NULL;
}

COverlayWnd::~COverlayWnd()
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

bool COverlayWnd::LoadFromResource(LPCTSTR pName, LPCTSTR pType)
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

void COverlayWnd::setDrawText(CString strText)
{
	m_strText = strText;
}
void COverlayWnd::setDrawFont(CString strFont)
{
	m_strFont = strFont;
}

void COverlayWnd::setDrawColor(Gdiplus::Color textcolor)
{
	m_textcolor = textcolor;
}

void COverlayWnd::setDrawColor(COLORREF color)
{
    m_textcolor = Gdiplus::Color::MakeARGB(255, GetRValue(color), GetGValue(color), GetBValue(color));
}

void COverlayWnd::setDrawBg()
{
	LoadFromResource(MAKEINTRESOURCE(IDB_PNG1), _T("PNG"));
}

BEGIN_MESSAGE_MAP(COverlayWnd, CWnd)
    ON_WM_SIZE()
END_MESSAGE_MAP()

void COverlayWnd::OnSize(UINT nType, int cx, int cy)
{
   CWnd::OnSize(nType, cx, cy);
   if (nType != SIZE_MINIMIZED)		
	   UpdateDrawText();
}

void COverlayWnd::UpdateDrawText()
{
	CRect rClient;
    GetClientRect(&rClient);

    //화면 DC 생성
    CClientDC dcScreen(NULL);

    //Memory DC 생성
    CDC memDC;
	if (!memDC.CreateCompatibleDC(&dcScreen)) return;

    // Alpha 채널 포함된 DIBSection 비트맵 생성
    BITMAPINFO bmpInfo = { 0 };
    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biWidth = rClient.Width();
    bmpInfo.bmiHeader.biHeight = -rClient.Height(); // Top-down
    bmpInfo.bmiHeader.biPlanes = 1;
    bmpInfo.bmiHeader.biBitCount = 32;
    bmpInfo.bmiHeader.biCompression = BI_RGB;

    void* pBits = NULL;
    HBITMAP hBmp = CreateDIBSection(dcScreen, &bmpInfo, DIB_RGB_COLORS, &pBits, NULL, 0);
	if (!hBmp) return;
    
	CBitmap bmp;
    bmp.Attach(hBmp);
    CBitmap* pOldBmp = memDC.SelectObject(&bmp);

    //GDI+ 그래픽 객체 생성
    Graphics graphics(memDC);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);
    graphics.SetPixelOffsetMode(PixelOffsetModeHighQuality);

	//투명 배경
    //graphics.Clear(Color(0, 0, 0, 0));

	//불투명 배경
    //graphics.Clear(Color(255, 0, 0, 0));
	
	//배경이미지 적용
	if(m_pBitmap)
	{
		graphics.DrawImage(m_pBitmap, 0, 0, rClient.Width(), rClient.Height());
	}
	else
	{
		graphics.Clear(Color(0, 0, 0, 0));
	}
	
    //텍스트
    DrawText(graphics, _T(m_strText),m_strFont, 40, FontStyleBold,
        RectF(0.0f, 0.0f, 460.0f, 40.0f), StringAlignmentFar, m_textcolor, Color(0, 0, 0, 0));

    //Alpha 블렌딩 처리
    POINT ptSrc = { 0, 0 };
    SIZE sizeWnd = { rClient.Width(), rClient.Height() };
    BLENDFUNCTION blend = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

    ::UpdateLayeredWindow(GetSafeHwnd(), dcScreen.GetSafeHdc(), NULL, &sizeWnd,
        memDC.GetSafeHdc(), &ptSrc, 0, &blend, ULW_ALPHA);

    memDC.SelectObject(pOldBmp);
	::DeleteObject(bmp.Detach());
}

void COverlayWnd::DrawText(Graphics& graphics,
                                   CString strText,
                                   CString strFontName,
                                   REAL fontSize,
                                   INT fontStyle,
                                   RectF fRect,
                                   StringAlignment alignment,
                                   Color textcolor,
								   Gdiplus::Color bgcolor)
{	
	USES_CONVERSION;

	//배경색
	//SolidBrush backgroundBrush(bgcolor);
    //graphics.FillRectangle(&backgroundBrush, fRect);

    Gdiplus::FontFamily fontFamily(A2W(strFontName));
    Gdiplus::Font font(&fontFamily, fontSize, fontStyle, UnitPixel);

    Gdiplus::StringFormat format;
    format.SetAlignment(alignment);
    format.SetLineAlignment(StringAlignmentNear); //수직 정렬은 상단 기준

    Gdiplus::SolidBrush brush(textcolor);
    graphics.DrawString(A2W(strText), -1, &font, fRect, &format, &brush);
}
