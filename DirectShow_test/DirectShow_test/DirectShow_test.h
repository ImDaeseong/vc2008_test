#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

class CDirectShow_testApp : public CWinAppEx
{
public:
	CDirectShow_testApp();

	public:
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()
};

extern CDirectShow_testApp theApp;