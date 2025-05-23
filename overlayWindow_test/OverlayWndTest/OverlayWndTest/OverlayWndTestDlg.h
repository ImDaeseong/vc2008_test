#pragma once
#include "explorer1.h"

class COverlayWndTestDlg : public CDialog
{
public:
	COverlayWndTestDlg(CWnd* pParent = NULL);	// standard constructor

	enum { IDD = IDD_OVERLAYWNDTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


protected:
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	afx_msg void OnPaint();	
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	DECLARE_MESSAGE_MAP()

public:
	void setBrowserMode(BOOL bShow);

	CExplorer1 m_browser;
	COverlayWnd* m_overWnd;	
};
