#include "stdafx.h"
#include "checkRedistInstall.h"
#include "checkRedistInstallDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CcheckRedistInstallApp, CWinAppEx)
END_MESSAGE_MAP()

CcheckRedistInstallApp::CcheckRedistInstallApp()
{
}

CcheckRedistInstallApp theApp;

BOOL CcheckRedistInstallApp::InitInstance()
{	
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	CcheckRedistInstallDlg dlg;
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
