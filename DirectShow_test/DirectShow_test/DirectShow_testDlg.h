#pragma once
#include "DirectShow.h"

class CDirectShow_testDlg : public CDialog
{
public:
	CDirectShow_testDlg(CWnd* pParent = NULL);	// standard constructor

	enum { IDD = IDD_DIRECTSHOW_TEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg LRESULT OnHandleEvent(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

private:
	DirectShow m_DirectShow;

public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
};
