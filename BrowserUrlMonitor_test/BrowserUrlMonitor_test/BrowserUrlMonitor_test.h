#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		

class CBrowserUrlMonitor_testApp : public CWinAppEx
{
public:
	CBrowserUrlMonitor_testApp();

	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();	
	DECLARE_MESSAGE_MAP()
};

extern CBrowserUrlMonitor_testApp theApp;