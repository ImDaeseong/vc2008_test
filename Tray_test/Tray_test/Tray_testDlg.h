#pragma once
#include "webbrowser2Ex.h"

class CTray_testDlg : public CDialog
{
public:
	CTray_testDlg(CWnd* pParent = NULL);	// standard constructor

	enum { IDD = IDD_TRAY_TEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

private:
	CGdiPlusBitmapResource *m_pbgImage;
	void LoadImage();
	void DrawSkin(CDC * pDC);
	int m_nWidth;
	int m_nHeight;
	
	void ShowTray();
	void HideTray();

	void InitWebBrowser();
	void NavigateA(CString strURL);
	CWebBrowser2Ex m_WebBrowser;
};
