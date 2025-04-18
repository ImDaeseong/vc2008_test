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
		
	CRect r1 = CRect(rcClient.left + 20, rcClient.top + 50, rcClient.left + 280, rcClient.top + 100);
	m_pTextStatic.Create(GetSafeHwnd(), r1, _T("텍스트 텍스트"), 30, _T("맑은 고딕"), FontStyleBold, RGB(255, 0, 0) );
	
	CRect r2 = CRect(rcClient.left + 20, rcClient.top + 120, rcClient.left + 200, rcClient.top + 160);	
	m_pngStatic.Create(GetSafeHwnd(), r2);
    m_pngStatic.LoadFromResource(MAKEINTRESOURCE(IDB_PNG2), _T("PNG"));
    m_pngStatic.Refresh();

	CRect r3 = CRect(rcClient.left + 20, rcClient.top + 200, rcClient.left + 120, rcClient.top + 230);	
	m_pButton.Create(GetSafeHwnd(), r3, _T("버튼 클릭"));
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
		
	CRect r1 = CRect(rcClient.left + 20, rcClient.top + 50, rcClient.left + 280, rcClient.top + 100);
	
    if (m_pTextStatic.GetSafeHwnd())
    {
        m_pTextStatic.MoveWindow(&r1);
    }

	CRect r2 = CRect(rcClient.left + 20, rcClient.top + 120, rcClient.left + 200, rcClient.top + 160);	
	
	if (m_pngStatic.GetSafeHwnd())
    {
        m_pngStatic.MoveWindow(&r2);
    }

	CRect r3 = CRect(rcClient.left + 20, rcClient.top + 200, rcClient.left + 120, rcClient.top + 230);	
	
	if (m_pButton.GetSafeHwnd())
    {
        m_pButton.MoveWindow(&r3);
    }
}

void COverlayWnd::OnDestroy()
{
	m_pTextStatic.DestroyWindow();
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
