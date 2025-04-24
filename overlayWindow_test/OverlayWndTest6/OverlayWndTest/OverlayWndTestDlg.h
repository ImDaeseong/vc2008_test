#pragma once

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

private: 
	void setBrowserModeEx(BOOL bShow);
	void closeBrowserModeEx(); 
	BOOL m_bShow;

	COverlayWndEx* m_overWndEx;
};
