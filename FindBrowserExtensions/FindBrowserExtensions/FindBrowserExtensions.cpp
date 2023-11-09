#include "stdafx.h"
#include "FindBrowserExtensions.h"
#include "FindBrowserExtensionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CFindBrowserExtensionsApp, CWinAppEx)
END_MESSAGE_MAP()

CFindBrowserExtensionsApp::CFindBrowserExtensionsApp()
{
}

CFindBrowserExtensionsApp theApp;

BOOL CFindBrowserExtensionsApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	CFindBrowserExtensionsDlg dlg;
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
