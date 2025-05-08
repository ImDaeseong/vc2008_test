#pragma once

class CLoL_testDlg : public CDialog
{
public:
	CLoL_testDlg(CWnd* pParent = NULL);	// standard constructor

	enum { IDD = IDD_LOL_TEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();	
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

private:
	CLoLManager* m_lolManager;
};
