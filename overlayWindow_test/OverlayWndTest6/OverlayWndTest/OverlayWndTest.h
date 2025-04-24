#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

class COverlayWndTestApp : public CWinAppEx
{
public:
	COverlayWndTestApp();

	public:
	virtual BOOL InitInstance();
    virtual int ExitInstance();
	DECLARE_MESSAGE_MAP()

private:
	ULONG_PTR m_gdiplusToken;
};

extern COverlayWndTestApp theApp;