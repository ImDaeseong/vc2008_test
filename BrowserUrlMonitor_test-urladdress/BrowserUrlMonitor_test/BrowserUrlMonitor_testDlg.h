#pragma once
#include "Resource.h"

class CBrowserUrlMonitor;
class CBrowserUrlMonitor_testDlg : public CDialog
{
public:
    CBrowserUrlMonitor_testDlg(CWnd* pParent = NULL);
    enum { IDD = IDD_BROWSERURLMONITOR_TEST_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    virtual BOOL DestroyWindow();
    afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
    DECLARE_MESSAGE_MAP()

private:
    CBrowserUrlMonitor* m_pUrlMonitor;
};