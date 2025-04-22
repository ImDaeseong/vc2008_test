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

	SetTimer(1, 5000, NULL);

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

void COverlayWndTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	setBrowserMode(m_bShow);
	m_bShow = !m_bShow; 
}

void COverlayWndTestDlg::setBrowserMode(BOOL bShow)
{
	CRect rRect;
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &rRect, 0);

	if(bShow)
	{
		int nWidth = 460;
		int nHeight = 40;

		//중앙 위치
		int nLeft = rRect.left + (rRect.Width() - nWidth) / 2;
		int nTop = rRect.top + 10;//(rRect.Height() - nHeight) / 2;
		CRect rRc(nLeft, nTop, nLeft + nWidth, nTop + nHeight);
		
		if(m_overWnd == NULL)
		{
			//정적 변수로 한 번만 등록
			static LPCTSTR s_overlayClass = AfxRegisterWndClass(0);

			m_overWnd = new COverlayWnd();
			m_overWnd->CreateEx(WS_EX_NOACTIVATE | WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW, 
				s_overlayClass, NULL, WS_POPUP|WS_VISIBLE, rRc, NULL, 0);
			
			m_overWnd->setDrawText(_T("최상위 비활성화 원도우"));
			m_overWnd->setDrawFont(_T("돋움"));
			m_overWnd->setDrawColor(RGB(255, 0, 0));
			m_overWnd->UpdateDrawText();
			m_overWnd->MoveWindow(rRc);
		}
		else
		{
			m_overWnd->MoveWindow(rRc); 
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
