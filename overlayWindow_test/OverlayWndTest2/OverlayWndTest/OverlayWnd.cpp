#include "StdAfx.h"
#include "OverlayWnd.h"
#include "resource.h"

COverlayWnd* COverlayWnd::m_wndOverlay = NULL;

COverlayWnd::COverlayWnd()
{
}

COverlayWnd::~COverlayWnd()
{
}

BEGIN_MESSAGE_MAP(COverlayWnd, CWnd)	 
	ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_WM_SIZE()
    ON_WM_MOVE()
END_MESSAGE_MAP()

int COverlayWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

	m_wndOverlay = this;

    CRect rcClient;
    GetClientRect(&rcClient);
	ClientToScreen(&rcClient); 
		
	CRect r1 = CRect(rcClient.left + 10, rcClient.top + 10, rcClient.left + 240, rcClient.top + 50);
	m_pTextStatic1.Create(GetSafeHwnd(), r1, _T("텍스트 텍스트1"), 30, _T("맑은 고딕"), FontStyleBold, RGB(255, 0, 0) );
	
	CRect r2 = CRect(rcClient.left + 10, rcClient.top + 60, rcClient.left + 240, rcClient.top + 100);
	m_pTextStatic2.Create(GetSafeHwnd(), r2, _T("텍스트 텍스트2"), 30, _T("맑은 고딕"), FontStyleBold, RGB(255, 0, 0) );
	
	CRect r3 = CRect(rcClient.left + 10, rcClient.top + 110, rcClient.left + 240, rcClient.top + 150);
	m_pTextStatic3.Create(GetSafeHwnd(), r3, _T("텍스트 텍스트3"), 30, _T("맑은 고딕"), FontStyleBold, RGB(255, 0, 0) );
	
	CRect r4 = CRect(rcClient.left + 20, rcClient.top + 320, rcClient.left + 200, rcClient.top + 360);	
	m_pngStatic.Create(GetSafeHwnd(), r4);
    
	CRect r5 = CRect(rcClient.left + 280, rcClient.top + 490, rcClient.left + 380, rcClient.top + 515);	
	m_pButton.Create(GetSafeHwnd(), r5, _T("버튼 클릭"));
	m_pButton.SetOnClick(OnOverlayWndButtonClick);

    return 0;
}

void COverlayWnd::OnOverlayWndButtonClick()
{
	if(m_wndOverlay)
	{
		m_wndOverlay->OnButtonExClick();
	}
}

void COverlayWnd::OnButtonExClick()
{
	AfxMessageBox(_T("버튼 클릭"));
}

void COverlayWnd::reMove()
{
	CRect rcClient;
    GetClientRect(&rcClient);
	ClientToScreen(&rcClient); 
	
	CRect r1 = CRect(rcClient.left + 10, rcClient.top + 10, rcClient.left + 240, rcClient.top + 50);
    if (m_pTextStatic1.GetSafeHwnd())
    {
        m_pTextStatic1.MoveWindow(&r1);
    }

	CRect r2 = CRect(rcClient.left + 10, rcClient.top + 60, rcClient.left + 240, rcClient.top + 100);
	if (m_pTextStatic2.GetSafeHwnd())
    {		
        m_pTextStatic2.MoveWindow(&r2);
    }

	CRect r3 = CRect(rcClient.left + 10, rcClient.top + 110, rcClient.left + 240, rcClient.top + 150);
	if (m_pTextStatic3.GetSafeHwnd())
    {		
        m_pTextStatic3.MoveWindow(&r3);
    }
	
	CRect r4 = CRect(rcClient.left + 20, rcClient.top + 320, rcClient.left + 200, rcClient.top + 360);
	if (m_pngStatic.GetSafeHwnd())
    {				
        m_pngStatic.MoveWindow(&r4);
    }
	
	CRect r5 = CRect(rcClient.left + 280, rcClient.top + 490, rcClient.left + 380, rcClient.top + 515);
	if (m_pButton.GetSafeHwnd())
    {		
        m_pButton.MoveWindow(&r5);
    }
}

void COverlayWnd::OnDestroy()
{
	m_pTextStatic1.DestroyWindow();
	m_pTextStatic2.DestroyWindow();
	m_pTextStatic3.DestroyWindow();
	m_pngStatic.DestroyWindow();
	m_pButton.DestroyWindow();

    CWnd::OnDestroy();
}

void COverlayWnd::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);

	reMove();
}

void COverlayWnd::OnMove(int x, int y)
{
    CWnd::OnMove(x, y);

	reMove();	
}

void COverlayWnd::UpdateDrawBackGround()
{
	CRect rClient;
    GetClientRect(&rClient);

    // 화면 DC 생성
    CClientDC dcScreen(NULL);

    // Memory DC 생성
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

    // GDI+ 그래픽 객체 생성
    Graphics graphics(memDC);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);
    graphics.SetPixelOffsetMode(PixelOffsetModeHighQuality);

    //투명 배경
    graphics.Clear(Color(0, 0, 0, 0));

	//불투명 배경
    //graphics.Clear(Color(255, 255, 0, 0));

    //Alpha 블렌딩 처리
    POINT ptSrc = { 0, 0 };
    SIZE sizeWnd = { rClient.Width(), rClient.Height() };
    BLENDFUNCTION blend = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

    ::UpdateLayeredWindow(GetSafeHwnd(), dcScreen.GetSafeHdc(), NULL, &sizeWnd,
        memDC.GetSafeHdc(), &ptSrc, 0, &blend, ULW_ALPHA);

    memDC.SelectObject(pOldBmp);
	::DeleteObject(bmp.Detach());
}
