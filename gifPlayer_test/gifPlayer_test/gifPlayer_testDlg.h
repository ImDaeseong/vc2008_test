#pragma once
#include "GifStatic.h"

class CgifPlayer_testDlg : public CDialog
{
public:
	CgifPlayer_testDlg(CWnd* pParent = NULL);	// standard constructor

	enum { IDD = IDD_GIFPLAYER_TEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

private:
	CGifStatic m_GifStatic;
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
