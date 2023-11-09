#pragma once


class CFindBrowserExtensionsDlg : public CDialog
{
public:
	CFindBrowserExtensionsDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

	enum { IDD = IDD_FINDBROWSEREXTENSIONS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

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
