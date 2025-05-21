#include "stdafx.h"
#include "OverlayWndTest.h"
#include "OverlayWndTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

COverlayWndTestDlg::COverlayWndTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COverlayWndTestDlg::IDD, pParent)
{
	m_overWndEx = NULL;
}

void COverlayWndTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COverlayWndTestDlg, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, &COverlayWndTestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &COverlayWndTestDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &COverlayWndTestDlg::OnBnClickedButton3)
END_MESSAGE_MAP()

BOOL COverlayWndTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	setBrowserModeEx();

	return TRUE;  
}

BOOL COverlayWndTestDlg::DestroyWindow()
{
	closeBrowserModeEx();

	return CDialog::DestroyWindow();
}

void COverlayWndTestDlg::OnPaint()
{
	CPaintDC dc(this);
}

void COverlayWndTestDlg::setBrowserModeEx()
{
	CRect rRect;
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &rRect, 0);

	int nWidth = 460;
	int nHeight = 80;

	//중앙 위치
	int nLeft = rRect.left + (rRect.Width() - nWidth) / 2;
	int nTop = rRect.top + 10;//(rRect.Height() - nHeight) / 2;
	CRect rRc(nLeft, nTop, nLeft + nWidth, nTop + nHeight);

	if(m_overWndEx == NULL)
	{
		//정적 변수로 한 번만 등록
		static LPCTSTR s_overlayClass = AfxRegisterWndClass(0);

		m_overWndEx = new COverlayWndEx();
		m_overWndEx->CreateEx( WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW,//WS_EX_NOACTIVATE | WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
			s_overlayClass, NULL, WS_POPUP|WS_VISIBLE, rRc, NULL, 0);
		
		m_overWndEx->setDrawBg();
		m_overWndEx->setDrawText(_T("최상위 비활성화"), _T("최상위 비활성화"));
		m_overWndEx->setDrawFont(_T("돋움"), _T("돋움"));
		m_overWndEx->setDrawColor(RGB(255, 0, 0), RGB(0, 255, 0));
		m_overWndEx->UpdateDrawText();
	}
	else
	{
		m_overWndEx->MoveWindow(rRc); 
	}
}

void COverlayWndTestDlg::closeBrowserModeEx()
{
	if(m_overWndEx)
	{
		if(m_overWndEx->GetSafeHwnd())
		{
			m_overWndEx->DestroyWindow();
			delete m_overWndEx;
			m_overWndEx = NULL;
		}
	}
}

void COverlayWndTestDlg::OnBnClickedButton1()
{
	//보이기
	if(m_overWndEx)
	{
		m_overWndEx->SetWindowTransparency(255);

		//m_overWndEx->ShowWindow(SW_SHOW);
	}
}

void COverlayWndTestDlg::OnBnClickedButton2()
{
	//숨기기
	if(m_overWndEx)
	{
		m_overWndEx->SetWindowTransparency(0);

		//m_overWndEx->ShowWindow(SW_HIDE);
	}
}

void COverlayWndTestDlg::OnBnClickedButton3()
{
	if(m_overWndEx)
	{
		//랜덤 색상
		srand((unsigned)time(NULL));
		COLORREF color1 = RGB(rand() % 256, rand() % 256, rand() % 256);
		COLORREF color2 = RGB(rand() % 256, rand() % 256, rand() % 256);

		int nRand = rand() % 1000;

		CString strText;
		strText.Format(_T("최상위 비활성화%d"), nRand);

		m_overWndEx->setDrawText(strText, strText);
		m_overWndEx->setDrawFont(_T("돋움"), _T("돋움"));
		m_overWndEx->setDrawColor(color1, color2);
		m_overWndEx->UpdateDrawText();
	}
}
