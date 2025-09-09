#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

class Cmysqlconnect_testApp : public CWinAppEx
{
public:
	Cmysqlconnect_testApp();

	public:
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()
};

extern Cmysqlconnect_testApp theApp;