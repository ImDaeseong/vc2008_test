#include "stdafx.h"
#include "BrowserUrlMonitor_test.h"
#include "BrowserUrlMonitor_testDlg.h"
#include "BrowserUrlMonitor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CBrowserUrlMonitor_testDlg::CBrowserUrlMonitor_testDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBrowserUrlMonitor_testDlg::IDD, pParent)
{
}

void CBrowserUrlMonitor_testDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBrowserUrlMonitor_testDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CBrowserUrlMonitor_testDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetTimer(1, 1000, NULL);

	return TRUE; 
}

void CBrowserUrlMonitor_testDlg::OnPaint()
{
	CPaintDC dc(this);
}

BOOL CBrowserUrlMonitor_testDlg::DestroyWindow()
{
	CBrowserUrlMonitor::ReleaseInstance();

	return CDialog::DestroyWindow();
}

void CBrowserUrlMonitor_testDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		HWND hForeground = ::GetForegroundWindow();
		if (hForeground != NULL)
		{
			CBrowserUrlMonitor* pMonitor = CBrowserUrlMonitor::GetInstance();
			
			if (pMonitor && pMonitor->IsSupportedBrowser(hForeground))
			{
				CString szUrl;

				if (pMonitor->GetLastKnownUrl(hForeground, szUrl))
				{
					CString strMsg;
					strMsg.Format(_T("°¨ÁöµÈ URL: %s\n"),szUrl);
		
					SetDlgItemText(IDC_EDIT1, _T(""));
					SetDlgItemText(IDC_EDIT1, strMsg);

				}
			}
		}
	}

	CDialog::OnTimer(nIDEvent);
}
