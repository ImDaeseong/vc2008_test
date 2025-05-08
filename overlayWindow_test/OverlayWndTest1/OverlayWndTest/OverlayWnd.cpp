#include "StdAfx.h"
#include "OverlayWnd.h"

COverlayWnd::COverlayWnd()
{
	m_strText1 = "";
	m_strText2 = "";
	m_strText3 = "";

	m_strFont1 = _T("돋움");
	m_strFont2 = _T("돋움");
	m_strFont3 = _T("돋움");

	m_textcolor1 = Color(255, 0, 0, 0);
	m_textcolor2 = Color(255, 0, 0, 0);
	m_textcolor3 = Color(255, 0, 0, 0);
}

void COverlayWnd::setDrawText(CString strText1, CString strText2, CString strText3)
{
	m_strText1 = strText1;
	m_strText2 = strText2;
	m_strText3 = strText3;
}
void COverlayWnd::setDrawFont(CString strFont1, CString strFont2, CString strFont3)
{
	m_strFont1 = strFont1;
	m_strFont2 = strFont2;
	m_strFont3 = strFont3;
}

void COverlayWnd::setDrawColor(Gdiplus::Color textcolor1, Gdiplus::Color textcolor2, Gdiplus::Color textcolor3)
{
	m_textcolor1 = textcolor1;
	m_textcolor2 = textcolor2;
	m_textcolor3 = textcolor3;	
}

void COverlayWnd::setDrawColor(COLORREF color1, COLORREF color2, COLORREF color3)
{
    m_textcolor1 = Gdiplus::Color::MakeARGB(255, GetRValue(color1), GetGValue(color1), GetBValue(color1));
    m_textcolor2 = Gdiplus::Color::MakeARGB(255, GetRValue(color2), GetGValue(color2), GetBValue(color2));
    m_textcolor3 = Gdiplus::Color::MakeARGB(255, GetRValue(color3), GetGValue(color3), GetBValue(color3));
}

COverlayWnd::~COverlayWnd()
{
}

BEGIN_MESSAGE_MAP(COverlayWnd, CWnd)
    ON_WM_SIZE()
	ON_WM_ERASEBKGND()
    ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

void COverlayWnd::OnSize(UINT nType, int cx, int cy)
{
   CWnd::OnSize(nType, cx, cy);
   if (nType != SIZE_MINIMIZED)		
	   UpdateDrawText();
}

BOOL COverlayWnd::OnEraseBkgnd(CDC* pDC)
{
    //깜빡임 방지
    return TRUE;
}

void COverlayWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
    //클릭 무시
}

BOOL COverlayWnd::PreTranslateMessage(MSG* pMsg)
{    
	switch (pMsg->message)
    {
    case WM_KEYDOWN:
        if (pMsg->wParam == VK_ESCAPE)
        {
            //ESC 키 입력 무시
            return TRUE;
        }
        break;

    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_MOUSEMOVE:
        //마우스 클릭 및 이동 무시
        return TRUE;
    }

    return CWnd::PreTranslateMessage(pMsg); 
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
    graphics.Clear(Color(0, 0, 0, 0));

	//불투명 배경
    //graphics.Clear(Color(255, 0, 0, 0));

    //텍스트
    DrawText(graphics, _T(m_strText1),m_strFont1, 40, FontStyleBold,
        RectF(20.0f, 40.0f, 110.0f, 60.0f), StringAlignmentFar,
        m_textcolor1, Color(0, 0, 0, 0));

    DrawText(graphics, _T(m_strText2), m_strFont2, 24, FontStyleBold,
        RectF(140.0f, 42.0f, 280.0f, 24.0f), StringAlignmentNear,
        m_textcolor2, Color(0, 0, 0, 0));

    DrawText(graphics, _T(m_strText3), m_strFont3, 11, FontStyleRegular,
        RectF(140.0f, 76.0f, 280.0f, 24.0f), StringAlignmentNear,
        m_textcolor3, Color(0, 0, 0, 0));

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
