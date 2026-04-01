#include "stdafx.h"
#include "BrowserUrlMonitor_test.h"
#include "BrowserUrlMonitor_testDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CBrowserUrlMonitor_testApp, CWinAppEx)
END_MESSAGE_MAP()

CBrowserUrlMonitor_testApp::CBrowserUrlMonitor_testApp()
{
}

CBrowserUrlMonitor_testApp theApp;

BOOL CBrowserUrlMonitor_testApp::InitInstance()
{	
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if (FAILED(hr) && hr != RPC_E_CHANGED_MODE)
    {
        TRACE(_T("COM 초기화 실패: 0x%08X\n"), hr);
    }

	CBrowserUrlMonitor_testDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	return FALSE;
}

int CBrowserUrlMonitor_testApp::ExitInstance()
{
	CoUninitialize();

	return CWinAppEx::ExitInstance();
}
