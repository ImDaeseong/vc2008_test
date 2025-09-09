#pragma once

class Cmysqlconnect_testDlg : public CDialog
{
public:
	Cmysqlconnect_testDlg(CWnd* pParent = NULL);	// standard constructor

	enum { IDD = IDD_MYSQLCONNECT_TEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

private:
	BOOL InitDB();
};
