#include "stdafx.h"
#include "OverlayWndTest.h"
#include "OverlayWndTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

COverlayWndTestDlg::COverlayWndTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COverlayWndTestDlg::IDD, pParent)
{
	m_overWnd = NULL;
	m_bShow = TRUE;
}

void COverlayWndTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COverlayWndTestDlg, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, &COverlayWndTestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &COverlayWndTestDlg::OnBnClickedButton2)
END_MESSAGE_MAP()

BOOL COverlayWndTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	setBrowserMode(TRUE);
	
	return TRUE;  
}

BOOL COverlayWndTestDlg::DestroyWindow()
{
	closeBrowserMode();
	
	return CDialog::DestroyWindow();
}

void COverlayWndTestDlg::OnPaint()
{
	CPaintDC dc(this);
}

void COverlayWndTestDlg::setBrowserMode(BOOL bShow)
{
	m_bShow = bShow;

	CRect rRect;
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &rRect, 0);

	if(bShow)
	{
		int nWidth = 800;
		int nHeight = 80;

		//중앙 위치
		int nLeft = rRect.left + (rRect.Width() - nWidth) / 2;
		int nTop = rRect.top + 10;//(rRect.Height() - nHeight) / 2;
		CRect rRc(nLeft, nTop, nLeft + nWidth, nTop + nHeight);
		

		//문자열 정보
		std::vector<TextInfo> vLine;

		TextInfo info1;
		info1.strText = _T("최상위 비활성화 최상위 비활성화 최상위 비활성화");
		info1.nFontSize = 30;
		info1.clrColor = D3DCOLOR_ARGB(255, 255, 0, 0);
		info1.nFontWeight = FW_BOLD;
		vLine.push_back(info1);

		TextInfo info2;
		info2.strText = _T("최상위 비활성화 최상위 비활성화");
		info2.nFontSize = 25;
		info2.clrColor = D3DCOLOR_ARGB(255, 255, 205, 0);
		info2.nFontWeight = FW_NORMAL;
		vLine.push_back(info2);
		
		if(m_overWnd == NULL)
		{
			static LPCTSTR s_overlayClass = AfxRegisterWndClass(0);
			m_overWnd = new COverlayWnd();
			m_overWnd->CreateEx(
			WS_EX_NOACTIVATE | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_TOPMOST,
			s_overlayClass, NULL, WS_POPUP, rRc, NULL, 0);
			
			m_overWnd->addText(vLine);
	    		
			//윈도우 투명도 설정
			m_overWnd->SetLayeredWindowAttributes(RGB(0,0,0), 0, LWA_COLORKEY);

			m_overWnd->ShowWindow(SW_SHOWNORMAL);
    		m_overWnd->UpdateWindow();			
		}
		else
		{
			m_overWnd->MoveWindow(rRc); 
		}
	}
	else
	{
		closeBrowserMode();			
	}
}

void COverlayWndTestDlg::closeBrowserMode()
{
	if(m_overWnd)
	{
		if(m_overWnd->GetSafeHwnd())
		{
			m_overWnd->DestroyWindow();
			delete m_overWnd;
			m_overWnd = NULL;
		}
	}
}

void COverlayWndTestDlg::OnBnClickedButton1()
{
	if(m_overWnd)
	{
		m_overWnd->SetLayeredWindowAttributes(RGB(0,0,0), 0, LWA_COLORKEY);
	}
	//setBrowserMode(TRUE);
}

void COverlayWndTestDlg::OnBnClickedButton2()
{
	if(m_overWnd)
	{
		m_overWnd->SetLayeredWindowAttributes(0, 0, LWA_ALPHA);
	}
	//setBrowserMode(FALSE);
}
