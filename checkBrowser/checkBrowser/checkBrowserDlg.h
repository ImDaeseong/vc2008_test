#pragma once
#include "resource.h"

class CcheckBrowserDlg : public CDialog
{
public:
	CcheckBrowserDlg(CWnd* pParent = NULL);	// standard constructor

	enum { IDD = IDD_CHECKBROWSER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();	
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()
	
private:
	void FindBrowserPIDs();
	void FindBrowserPIDsTime();
	DWORD FindBrowserPIDsLast();

	UINT_PTR m_timer;

public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
