#include "stdafx.h"
#include "Tray_test.h"
#include "Tray_testDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CTray_testDlg::CTray_testDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTray_testDlg::IDD, pParent)
{
	m_pbgImage = NULL;

	m_nWidth = 0;
	m_nHeight = 0;
}

void CTray_testDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER1, m_WebBrowser);
}

BEGIN_MESSAGE_MAP(CTray_testDlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CTray_testDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}

void CTray_testDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		KillTimer(1);
		ShowTray();
	}
	else if (nIDEvent == 2)
	{
		KillTimer(2);
		HideTray();		
		CDialog::OnOK();	
	}
}

void CTray_testDlg::OnDestroy() 
{
	if(m_pbgImage != NULL)
		delete m_pbgImage;
	m_pbgImage = NULL;	
}

BOOL CTray_testDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW, 0);

	BringToForeground();

	LoadImage();

	MoveWindow(-3000, -3000, 0, 0);

	InitWebBrowser();

	SetTimer(1, 1000, NULL);
	SetTimer(2, 5000, NULL);

	return TRUE;  
}

void CTray_testDlg::OnPaint()
{
	CPaintDC dc(this);
	DrawSkin(&dc);
}

void CTray_testDlg::LoadImage()
{
	if (!m_pbgImage)
	{
		m_pbgImage = new CGdiPlusBitmapResource;
		if (!m_pbgImage->Load(MAKEINTRESOURCE(IDB_PNG_BG), "PNG", AfxGetApp()->m_hInstance))
			return;
	}

	int x = m_pbgImage->m_pBitmap->GetWidth();
	int y = m_pbgImage->m_pBitmap->GetHeight();		
	MoveWindow(0, 0, x, y);

	m_nWidth = x;
	m_nHeight = y;
}

void CTray_testDlg::DrawSkin(CDC *pDC)
{
	int x = m_pbgImage->m_pBitmap->GetWidth();
	int y = m_pbgImage->m_pBitmap->GetHeight();

	Graphics gps(pDC->GetSafeHdc());
	gps.DrawImage(m_pbgImage->m_pBitmap, Rect(0, 0, x, y), 0, 0, x, y, UnitPixel);
}

void CTray_testDlg::InitWebBrowser()
{	
	m_WebBrowser.MoveWindow(10, 10, 476, 332);

	NavigateA("https://www.naver.com");
}

void CTray_testDlg::NavigateA(CString strURL)
{		
	VARIANT vtEmpty;
	::VariantInit(&vtEmpty);
	
	m_WebBrowser.Navigate(strURL, &vtEmpty, &vtEmpty, &vtEmpty, &vtEmpty);
}

void CTray_testDlg::BringToForeground()
{
	if (::GetForegroundWindow() == m_hWnd)
		return;

	HWND hActiveWnd = ::GetForegroundWindow();
	if (!hActiveWnd)
		return;

	DWORD activeThreadId = GetWindowThreadProcessId(hActiveWnd, NULL);
	DWORD currentThreadId = GetCurrentThreadId();

	if (currentThreadId != activeThreadId)
	{
		if (AttachThreadInput(currentThreadId, activeThreadId, TRUE))
		{
			BringWindowToTop();

			//포커스 필요시 사용
			//SetForegroundWindow();

			AttachThreadInput(currentThreadId, activeThreadId, FALSE);
		}
	}
}

void CTray_testDlg::ShowTray()
{
	int nScreenSizeX = GetSystemMetrics(SM_CXSCREEN);
	int nScreenSizeY = GetSystemMetrics(SM_CYSCREEN);

	MoveWindow(nScreenSizeX, nScreenSizeY, m_nWidth, m_nHeight);

	::ShowTray(this->m_hWnd, 1000, TRUE, TRUE);
}

void CTray_testDlg::HideTray()
{
	::ShowTray(this->m_hWnd, 500, FALSE, TRUE);
}