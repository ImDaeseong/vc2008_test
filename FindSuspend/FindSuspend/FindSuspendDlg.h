#pragma once
#include "afxcmn.h"
#include "CheckSuspend.h"

class CFindSuspendDlg : public CDialog
{
public:
	CFindSuspendDlg(CWnd* pParent = NULL);	// standard constructor

	enum { IDD = IDD_FINDSUSPEND_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()

private:
	CListCtrl m_ListCtrl;

	void InitList();	 
	void RefreshProcessList();
    void ToggleProcessState(int index);
    void InsertList(const CString& strProcess, const CString& strState, DWORD processID);
    void UpdateProcessState(int index);
		
	CheckSuspend m_CheckSuspend;
    std::vector<DWORD> m_ProcessIDs;
};
