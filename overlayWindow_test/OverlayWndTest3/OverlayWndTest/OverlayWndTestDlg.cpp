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
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &COverlayWndTestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &COverlayWndTestDlg::OnBnClickedButton2)
END_MESSAGE_MAP()

BOOL COverlayWndTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	setBrowserMode(TRUE);

	SetTimer(1, 5000, NULL);
	SetTimer(2, 1000, NULL);

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

	KillTimer(1);
	KillTimer(2);

	return CDialog::DestroyWindow();
}

void COverlayWndTestDlg::OnPaint()
{
	CPaintDC dc(this);
}

void COverlayWndTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	if( nIDEvent == 1 )
	{
		setBrowserMode(m_bShow);
		m_bShow = !m_bShow; 
	}
	else if( nIDEvent == 2 )
	{
		if(m_overWnd)
		{
			if (IsTopmostWindow(m_overWnd->GetSafeHwnd()))
			{
				//�ٸ� TOPMOST �����찡 ����
			}
			else
			{
				//m_overWnd�� �ֻ��� TOPMOST ������
			}		
		}	
	}
}

BOOL COverlayWndTestDlg::IsTopmostWindow(HWND hCheckWnd)
{
    HWND hTopWnd = ::GetTopWindow(NULL);
	while (hTopWnd)
	{
		if (hTopWnd == hCheckWnd)
			return FALSE;

		LONG_PTR exStyle = ::GetWindowLongPtr(hTopWnd, GWL_EXSTYLE);
		if ((exStyle & WS_EX_TOPMOST) && ::IsWindowVisible(hTopWnd))
			return TRUE;

		hTopWnd = ::GetNextWindow(hTopWnd, GW_HWNDNEXT);
	}
	return FALSE;
}

void COverlayWndTestDlg::setBrowserMode(BOOL bShow)
{
	m_bShow = bShow;

	CRect rRect;
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &rRect, 0);

	if(bShow)
	{
		int nWidth = 460;
		int nHeight = 40;

		//�߾� ��ġ
		int nLeft = rRect.left + (rRect.Width() - nWidth) / 2;
		int nTop = rRect.top + 10;//(rRect.Height() - nHeight) / 2;
		CRect rRc(nLeft, nTop, nLeft + nWidth, nTop + nHeight);
		
		if(m_overWnd == NULL)
		{
			//���� ������ �� ���� ���
			static LPCTSTR s_overlayClass = AfxRegisterWndClass(0);

			m_overWnd = new COverlayWnd();
			m_overWnd->CreateEx(WS_EX_NOACTIVATE | WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW, 
				s_overlayClass, NULL, WS_POPUP|WS_VISIBLE, rRc, NULL, 0);
			
			m_overWnd->setDrawBg();
			m_overWnd->setDrawText(_T("�ֻ��� ��Ȱ��ȭ ������"));
			m_overWnd->setDrawFont(_T("����"));
			m_overWnd->setDrawColor(RGB(255, 0, 0));
			m_overWnd->UpdateDrawText();
			m_overWnd->MoveWindow(rRc);
		}
		else
		{
			m_overWnd->MoveWindow(rRc); 
		}

		/*
		//FullScreen TOPMOST �����찡 �ִ� ��� �ʿ�
		::SetWindowPos(m_overWnd->GetSafeHwnd(), HWND_TOPMOST, 0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
        */
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
