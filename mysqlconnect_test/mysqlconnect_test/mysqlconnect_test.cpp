#include "stdafx.h"
#include "mysqlconnect_test.h"
#include "mysqlconnect_testDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(Cmysqlconnect_testApp, CWinAppEx)
END_MESSAGE_MAP()

Cmysqlconnect_testApp::Cmysqlconnect_testApp()
{
}

Cmysqlconnect_testApp theApp;

BOOL Cmysqlconnect_testApp::InitInstance()
{	
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	Cmysqlconnect_testDlg dlg;
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
