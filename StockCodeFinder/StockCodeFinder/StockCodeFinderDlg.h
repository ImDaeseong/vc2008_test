#pragma once
#include "afxcmn.h"


typedef struct _StockList_
{
	CString strCompany;
	CString strCode;

} STOCKLIST_DATA;

typedef struct _StockData_
{
	int nIndex;
	std::vector<STOCKLIST_DATA> vtrStock;
} STOCK_DATA;


class CStockCodeFinderDlg : public CDialog
{
public:
	CStockCodeFinderDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

	enum { IDD = IDD_STOCKCODEFINDER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();
	DECLARE_MESSAGE_MAP()

private:
	CString GetModulePath();
	CString Utf8ToCString(const std::string& utf8String);
	void initData();
	std::vector<STOCK_DATA> mStockDta;

	void InitCtrl();
	void InsertListView(CString strCompany, CString strCode, CString strIndex);
	CListCtrl m_ListCtrl;	

};
