#include "stdafx.h"
#include "closeBrowser.h"
#include "closeBrowserDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CcloseBrowserApp, CWinAppEx)
END_MESSAGE_MAP()

CcloseBrowserApp::CcloseBrowserApp()
{
}

CcloseBrowserApp theApp;

BOOL CcloseBrowserApp::InitInstance()
{	
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	CcloseBrowserDlg dlg;
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
