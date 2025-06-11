#include "stdafx.h"
#include "Tray_test.h"
#include "Tray_testDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CTray_testApp, CWinAppEx)
END_MESSAGE_MAP()

CTray_testApp::CTray_testApp()
{
	m_gdiplusToken = NULL;
}

CTray_testApp theApp;

BOOL CTray_testApp::InitInstance()
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

	CTray_testDlg dlg;
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

int CTray_testApp::ExitInstance() 
{
	if(m_gdiplusToken != NULL)
	{
		GdiplusShutdown(m_gdiplusToken);
		m_gdiplusToken = NULL;
	}

	CoUninitialize();
	
	return CWinApp::ExitInstance();
}

BOOL CTray_testApp::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{    
		if (pMsg->wParam == VK_F1)
		{
			return TRUE;
		}
	}	
	else if (pMsg->message == WM_SYSKEYDOWN)
	{
		if (pMsg->wParam == VK_F4)
		{
			return TRUE;
		}
	}
	return CWinApp::PreTranslateMessage(pMsg);
}