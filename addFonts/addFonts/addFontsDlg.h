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
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	DECLARE_MESSAGE_MAP()

private:
	void GetFontList();
	CString GetFontFolder();
	bool InstallFont(const CString& sFontFilePath, const CString& sFontName);
	bool UnInstallFont(const CString& sFontName);

	CString GetModulePath(LPCTSTR pszSubPath = _T(""));
	bool IsWin10Later();

	CListBox m_fontList;	
};
