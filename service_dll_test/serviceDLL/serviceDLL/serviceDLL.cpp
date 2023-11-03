#include "stdafx.h"
#include "serviceDLL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(CserviceDLLApp, CWinApp)
END_MESSAGE_MAP()


CserviceDLLApp::CserviceDLLApp()
{
}

CserviceDLLApp theApp;

BOOL CserviceDLLApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}