#pragma once

#include "TextStatic.h"
#include "PngStatic.h"
#include "ButtonEx.h"

class COverlayWnd : public CWnd
{
public:
	COverlayWnd();
	~COverlayWnd();

	void UpdateDrawBackGround();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnDestroy();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnMove(int x, int y);
	DECLARE_MESSAGE_MAP()

private:	
	CTextStatic m_pTextStatic1;
	CTextStatic m_pTextStatic2;
	CTextStatic m_pTextStatic3;
	CPngStatic m_pngStatic;
	CButtonEx m_pButton;

	void reMove();

private:
	static COverlayWnd* m_wndOverlay; 

public:
	static void OnOverlayWndButtonClick();
    void OnButtonExClick();
};
