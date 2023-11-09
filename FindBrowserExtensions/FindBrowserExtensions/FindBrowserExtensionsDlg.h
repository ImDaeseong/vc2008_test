#pragma once


class CFindBrowserExtensionsDlg : public CDialog
{
public:
	CFindBrowserExtensionsDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

	enum { IDD = IDD_FINDBROWSEREXTENSIONS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

private:
	CExtensionFinder* m_pFinder;

	void initExtension();
	void clearExtension();
};
