#pragma once

class CcloseBrowserDlg : public CDialog
{
public:
	CcloseBrowserDlg(CWnd* pParent = NULL);	// standard constructor

	enum { IDD = IDD_CLOSEBROWSER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();

private:
	BOOL CloseWebBrowserTerminate(const CString& strExeName);
	BOOL CloseWebBrowserClose(const CString& strExeName);
	void CcloseBrowserDlg::CloseAllBrowsers(BOOL bHwnd);
};
