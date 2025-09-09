#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

class CCheckfirewallApp : public CWinAppEx
{
public:
	CCheckfirewallApp();

	public:
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()
};

extern CCheckfirewallApp theApp;