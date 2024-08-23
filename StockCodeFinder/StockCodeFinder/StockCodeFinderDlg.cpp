#include "stdafx.h"
#include "StockCodeFinder.h"
#include "StockCodeFinderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CStockCodeFinderDlg::CStockCodeFinderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStockCodeFinderDlg::IDD, pParent)
{
}

void CStockCodeFinderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}

BEGIN_MESSAGE_MAP(CStockCodeFinderDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CStockCodeFinderDlg::OnNMDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON1, &CStockCodeFinderDlg::OnBnClickedButton1)
END_MESSAGE_MAP()

BOOL CStockCodeFinderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitCtrl();

	loadJson();

	return TRUE;  
}

void CStockCodeFinderDlg::OnPaint()
{
	CPaintDC dc(this);
}

void CStockCodeFinderDlg::OnDestroy()
{
	CDialog::OnDestroy();

	mStockDta.clear();
}

CString CStockCodeFinderDlg::GetModulePath()
{
    TCHAR tPath[MAX_PATH];
    ::GetModuleFileName(NULL, tPath, MAX_PATH);

    CString strPath = tPath;
    int pos = strPath.ReverseFind('\\');
    if (pos != -1) {
        strPath = strPath.Left(pos + 1);
    }
    return strPath;
}

CString CStockCodeFinderDlg::Utf8ToCString(const std::string& utf8String)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, utf8String.c_str(), -1, NULL, 0);
    if (len == 0)
        return CString();

    std::vector<wchar_t> wstr(len);
    MultiByteToWideChar(CP_UTF8, 0, utf8String.c_str(), -1, &wstr[0], len);
    return CString(&wstr[0]);
}

void CStockCodeFinderDlg::loadJson()
{
	CString strPath = GetModulePath();
    strPath.Append(_T("code.json"));

	std::ifstream file(CT2CA(strPath, CP_UTF8)); 
    if (!file.is_open())
		return;


    Json::Reader reader;
    Json::Value json;
    std::string line;
	int i = 0;

    while (std::getline(file, line))
    {
        //JSON °´Ã¼ ÆÄ½Ì
        std::stringstream ss(line);
        if (reader.parse(ss, json))
        {
			i++;

            std::string company = json["company"].asString();
            std::string code = json["code"].asString();

            CString strCompany = Utf8ToCString(company);
            CString strCode = Utf8ToCString(code);

			STOCKLIST_DATA item;
			item.strCompany = strCompany;
			item.strCode = strCode;

			STOCK_DATA stock;
			stock.nIndex = i;
			stock.vtrStock.push_back(item);

			mStockDta.push_back(stock);
        }
    }
    file.close();	
}

void CStockCodeFinderDlg::InitCtrl()
{
	CRect rect;
	m_ListCtrl.GetClientRect(&rect);
	m_ListCtrl.InsertColumn(0, _T("È¸»ç¸í"), LVCFMT_CENTER, 300, -1);
	m_ListCtrl.InsertColumn(1, _T("ÄÚµå¸í"), LVCFMT_LEFT, rect.Width() - 300);
	m_ListCtrl.InsertColumn(2, _T(""), LVCFMT_LEFT, 0);
}

void CStockCodeFinderDlg::InsertListView(CString strCompany, CString strCode, CString strIndex)
{
	int nIndex = m_ListCtrl.InsertItem(0, strCompany);
	m_ListCtrl.SetItemText(nIndex, 1, strCode);
	m_ListCtrl.SetItemText(nIndex, 2, strIndex);
}

void CStockCodeFinderDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	CString strCompany = m_ListCtrl.GetItemText(pNMItemActivate->iItem, 0);
	CString strCode = m_ListCtrl.GetItemText(pNMItemActivate->iItem, 1);
	CString strIndex = m_ListCtrl.GetItemText(pNMItemActivate->iItem, 2);

	CString strMsg;
	strMsg.Format(_T("strCompany:%s strCode:%s strIndex:%s\n"), strCompany, strCode, strIndex);            
	OutputDebugString(strMsg);

	*pResult = 0;
}

void CStockCodeFinderDlg::OnBnClickedButton1()
{
	m_ListCtrl.DeleteAllItems();

	CString strIndex;

	for (std::vector<STOCK_DATA>::iterator it = mStockDta.begin(); it != mStockDta.end(); ++it)
	{	
		std::vector<STOCKLIST_DATA>::iterator itM;
		
		for (itM = it->vtrStock.begin(); itM != it->vtrStock.end(); ++itM)
		{			
			strIndex.Format(_T("%d"), it->nIndex);
			InsertListView(itM->strCompany, itM->strCode, strIndex);	
		}
	}
}
