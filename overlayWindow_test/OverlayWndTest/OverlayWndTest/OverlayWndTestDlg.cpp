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
}

void COverlayWndTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER1, m_browser);
}

BEGIN_MESSAGE_MAP(COverlayWndTestDlg, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, &COverlayWndTestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &COverlayWndTestDlg::OnBnClickedButton2)
END_MESSAGE_MAP()

BOOL COverlayWndTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;  
}

BOOL COverlayWndTestDlg::DestroyWindow()
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

	return CDialog::DestroyWindow();
}

void COverlayWndTestDlg::OnPaint()
{
	CPaintDC dc(this);
}

void COverlayWndTestDlg::setBrowserMode(BOOL bShow)
{
	if(bShow)
	{
		CRect rRect;
		GetClientRect(&rRect);
		
		m_browser.MoveWindow(10 , 10, rRect.Width() - 100, rRect.Height() - 40);
			
		COleVariant vtEmpty;		
		m_browser.Navigate("https://www.naver.com", &vtEmpty, &vtEmpty, &vtEmpty, &vtEmpty);


		CRect rcBrowser;
        m_browser.GetWindowRect(&rcBrowser);
		
		CRect rcWeb(rcBrowser.left + 10, rcBrowser.top + 10, rcBrowser.left + 400, rcBrowser.top + 100);
		
		if(m_overWnd != NULL)
		{
			m_overWnd->MoveWindow(rcWeb);  	
		}
		else
		{
			//정적 변수로 한 번만 등록
			static LPCTSTR s_overlayClass = AfxRegisterWndClass(0);

			m_overWnd = new COverlayWnd();
			m_overWnd->CreateEx(WS_EX_NOACTIVATE | WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW, 
				s_overlayClass, NULL, WS_POPUP|WS_VISIBLE, rcWeb, this, 0);

			m_overWnd->setDrawText(_T("1번째 텍스트"), _T("2번째 텍스트"), _T("3번째 텍스트"));
			m_overWnd->setDrawFont(_T("맑은 고딕"), _T("돋움"), _T("돋움"));
			//m_overWnd->setDrawColor(Color(255, 255, 0, 0), Color(255, 0, 255, 0), Color(255, 0, 0, 255));
			m_overWnd->setDrawColor(RGB(255, 0, 0), RGB(0, 255, 0), RGB(255, 255, 0));
			
			m_overWnd->UpdateDrawText();

			m_overWnd->MoveWindow(rcWeb);
		}		
	}
	else
	{
		m_browser.MoveWindow(-1000 , -1000, 0, 0);

		if(m_overWnd != NULL)
		{
			m_overWnd->MoveWindow(-1000, -1000, 0 ,0);
		}
	}
}

void COverlayWndTestDlg::OnBnClickedButton1()
{
	setBrowserMode(TRUE);
}

void COverlayWndTestDlg::OnBnClickedButton2()
{
	setBrowserMode(FALSE);
}
