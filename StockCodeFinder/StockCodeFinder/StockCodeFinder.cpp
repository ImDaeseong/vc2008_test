#include "stdafx.h"
#include "StockCodeFinder.h"
#include "StockCodeFinderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CStockCodeFinderApp, CWinAppEx)
END_MESSAGE_MAP()

CStockCodeFinderApp::CStockCodeFinderApp()
{
}

CStockCodeFinderApp theApp;

BOOL CStockCodeFinderApp::InitInstance()
{	
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();
	
	CStockCodeFinderDlg dlg;
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
