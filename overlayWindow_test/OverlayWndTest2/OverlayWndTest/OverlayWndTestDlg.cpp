#include "stdafx.h"
#include "OverlayWndTest.h"
#include "OverlayWndTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

COverlayWndTestDlg::COverlayWndTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COverlayWndTestDlg::IDD, pParent)
{
	m_browser = NULL;
	m_overWnd = NULL;
}

void COverlayWndTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_EXPLORER1, m_browser);
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

	if(m_browser)
	{
		if(m_browser->GetSafeHwnd())
		{
			m_browser->DestroyWindow();
			delete m_browser;
			m_browser = NULL;
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
	CRect rRect;		
	GetClientRect(&rRect);

	if(bShow)
	{
		CRect rRc(10 , 10, rRect.Width() - 100, rRect.Height() - 40);		

		if(m_browser == NULL)
		{
			m_browser = new CExplorer1();		
			m_browser->Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rRc, this, IDC_EXPLORER1);
			
			COleVariant vtEmpty;
			m_browser->Navigate("https://www.naver.com", &vtEmpty, &vtEmpty, &vtEmpty, &vtEmpty);
		}
		else
		{
			m_browser->MoveWindow(&rRc);
		}

		if(m_browser != NULL)
		{
			CRect rcBrowser;
			m_browser->GetWindowRect(&rcBrowser);
			CRect rcWeb(rcBrowser.left + 10, rcBrowser.top + 10, rcBrowser.left + 400, rcBrowser.top + 540);

			if(m_overWnd == NULL)
			{
				//정적 변수로 한 번만 등록
				static LPCTSTR s_overlayClass = AfxRegisterWndClass(0);

				m_overWnd = new COverlayWnd();
				m_overWnd->CreateEx(WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW, 
					s_overlayClass, NULL, WS_POPUP|WS_VISIBLE, rcWeb, this, 0);	
				m_overWnd->UpdateDrawBackGround();
				//m_overWnd->MoveWindow(rcWeb);
			}
			else
			{
				m_overWnd->MoveWindow(rcWeb); 
			}
		}		
	}
	else
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

		if(m_browser)
		{
			if(m_browser->GetSafeHwnd())
			{
				m_browser->DestroyWindow();
				delete m_browser;
				m_browser = NULL;
			}
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
