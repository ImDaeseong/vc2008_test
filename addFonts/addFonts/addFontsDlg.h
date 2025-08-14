#pragma once
#include "afxwin.h"

class CaddFontsDlg : public CDialog
{
public:
	CaddFontsDlg(CWnd* pParent = NULL);	// standard constructor

	enum { IDD = IDD_ADDFONTS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButton1();
	DECLARE_MESSAGE_MAP()

private:
	void GetFontList();
	BOOL InstallFont(const CString& sFontFilePath, const CString& sFontName);

	CListBox m_fontList;
};
