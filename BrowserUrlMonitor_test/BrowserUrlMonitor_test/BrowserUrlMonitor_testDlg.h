#pragma once
#include "Resource.h"
#include "BrowserUrlMonitor.h"

#define WM_URL_DETECTED (WM_USER + 100)

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
    afx_msg LRESULT OnUrlDetected(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()

private:
    CBrowserUrlMonitor* m_pUrlMonitor;
    BOOL InitUrlMonitor();
    void ReleaseUrlMonitor();
};