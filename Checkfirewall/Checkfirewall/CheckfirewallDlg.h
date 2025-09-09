#pragma once

class CCheckfirewallDlg : public CDialog
{
public:
	CCheckfirewallDlg(CWnd* pParent = NULL);	// standard constructor

	enum { IDD = IDD_CHECKFIREWALL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	DECLARE_MESSAGE_MAP()

private:
	BOOL AddFirewallRule(CString exePath, CString ruleName);
	BOOL DeleteFirewallRule(CString ruleName);
	CString GetModulePath();	
};
