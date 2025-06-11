#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

class CTray_testApp : public CWinAppEx
{
public:
	CTray_testApp();

	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()

	ULONG_PTR	m_gdiplusToken;
};

extern CTray_testApp theApp;