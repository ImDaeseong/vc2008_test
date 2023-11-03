#pragma once

typedef BOOL (*PFN_INSTALLSERVICE)(LPSTR);
typedef BOOL (*PFN_UNINSTALLSERVICE)();
typedef BOOL (*PFN_STARTSERVICE)();
typedef BOOL (*PFN_STOPSERVICE)();

class CserviceLoadDlg : public CDialog
{
public:
	CserviceLoadDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

	enum { IDD = IDD_SERVICELOAD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

private:
	HINSTANCE m_hDll;
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
};
