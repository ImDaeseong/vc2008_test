#include "stdafx.h"
#include "gifPlayer_test.h"
#include "gifPlayer_testDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CgifPlayer_testApp, CWinAppEx)
END_MESSAGE_MAP()

CgifPlayer_testApp::CgifPlayer_testApp()
{
	m_gdiplusToken = NULL;
}

CgifPlayer_testApp theApp;

BOOL CgifPlayer_testApp::InitInstance()
{	
	CoInitialize(NULL);

	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	CgifPlayer_testDlg dlg;
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

int CgifPlayer_testApp::ExitInstance()
{
	if(m_gdiplusToken != NULL)
	{
		GdiplusShutdown(m_gdiplusToken);
		m_gdiplusToken = NULL;
	}

	CoUninitialize();

	return CWinApp::ExitInstance();
}