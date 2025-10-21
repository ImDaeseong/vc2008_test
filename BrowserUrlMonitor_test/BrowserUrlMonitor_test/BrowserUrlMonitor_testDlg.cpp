#include "stdafx.h"
#include "BrowserUrlMonitor_test.h"
#include "BrowserUrlMonitor_testDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CBrowserUrlMonitor_testDlg::CBrowserUrlMonitor_testDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBrowserUrlMonitor_testDlg::IDD, pParent)
{
	m_pUrlMonitor = NULL;
}

void CBrowserUrlMonitor_testDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBrowserUrlMonitor_testDlg, CDialog)
	ON_WM_PAINT()
	ON_MESSAGE(WM_URL_DETECTED, &CBrowserUrlMonitor_testDlg::OnUrlDetected)
END_MESSAGE_MAP()

BOOL CBrowserUrlMonitor_testDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitUrlMonitor();

	return TRUE; 
}

void CBrowserUrlMonitor_testDlg::OnPaint()
{
	CPaintDC dc(this);
}

BOOL CBrowserUrlMonitor_testDlg::DestroyWindow()
{
	ReleaseUrlMonitor();

	return CDialog::DestroyWindow();
}

LRESULT CBrowserUrlMonitor_testDlg::OnUrlDetected(WPARAM, LPARAM lParam)
{    
	BrowserUrlInfo* pInfo = (BrowserUrlInfo*)lParam;
    if (pInfo)
    {
        CString strUrl = pInfo->strUrl;
        HWND hwnd = pInfo->hwndBrowser;

		char szTitle[MAX_PATH] = {0};
		::GetWindowText((HWND)hwnd, szTitle, MAX_PATH);

		CString strMsg;
		strMsg.Format(_T("宏扼快历 : %p\r\n力格 : %s\r\n林家 : %s\n"), hwnd, szTitle, strUrl);
		OutputDebugString(strMsg);

		SetDlgItemText(IDC_EDIT1, _T(""));
        SetDlgItemText(IDC_EDIT1, strMsg);

        delete pInfo;
    }
    return 0;
}

BOOL CBrowserUrlMonitor_testDlg::InitUrlMonitor()
{
	m_pUrlMonitor = new CBrowserUrlMonitor();	
    if (!m_pUrlMonitor->StartMonitor())
    {
        delete m_pUrlMonitor;
        m_pUrlMonitor = NULL;
        return FALSE;
    }
    return TRUE;
}

void CBrowserUrlMonitor_testDlg::ReleaseUrlMonitor()
{
	if (m_pUrlMonitor)
    {
        m_pUrlMonitor->StopMonitor();
        delete m_pUrlMonitor;
        m_pUrlMonitor = NULL;
    }
}