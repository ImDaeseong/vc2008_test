#include "stdafx.h"
#include "FindBrowserExtensions.h"
#include "FindBrowserExtensionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CFindBrowserExtensionsDlg::CFindBrowserExtensionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFindBrowserExtensionsDlg::IDD, pParent)
{
	m_pFinder = NULL;
}

void CFindBrowserExtensionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}

BEGIN_MESSAGE_MAP(CFindBrowserExtensionsDlg, CDialog)
	ON_WM_PAINT()	
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, &CFindBrowserExtensionsDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CFindBrowserExtensionsDlg::OnBnClickedButton2)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CFindBrowserExtensionsDlg::OnNMDblclkList1)
	ON_MESSAGE(WM_FINDEXTENSION_DONE, OnFindExtensionDone) 
	
END_MESSAGE_MAP()

BOOL CFindBrowserExtensionsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitCtrl();

	initExtension();

	return TRUE;  
}

void CFindBrowserExtensionsDlg::OnPaint()
{
	CPaintDC dc(this);
}

void CFindBrowserExtensionsDlg::OnDestroy()
{
	CDialog::OnDestroy();

	clearExtension();
}

void CFindBrowserExtensionsDlg::initExtension()
{
	// 초기화 전에 이전에 할당된 메모리를 해제
	clearExtension(); 

    m_pFinder = new CExtensionFinder(GetSafeHwnd());
    m_pFinder->StartExtensionFinder();
}

void CFindBrowserExtensionsDlg::clearExtension()
{
	if(m_pFinder)
	{
		m_pFinder->StopExtensionFinder();
		delete m_pFinder;
		m_pFinder = NULL;
	}
}

void CFindBrowserExtensionsDlg::OnBnClickedButton1()
{
	m_ListCtrl.DeleteAllItems();

	if(m_pFinder)
	{
		m_pFinder->StartExtensionFinder();
	}
}

void CFindBrowserExtensionsDlg::OnBnClickedButton2()
{
	CDialog::OnOK();
}

void CFindBrowserExtensionsDlg::InitCtrl()
{
	CRect rect;
	m_ListCtrl.GetClientRect(&rect);
	m_ListCtrl.InsertColumn(0, _T("항목"), LVCFMT_CENTER, 300, -1);
	m_ListCtrl.InsertColumn(1, _T("설명"), LVCFMT_LEFT, rect.Width() - 300);
	m_ListCtrl.InsertColumn(2, _T(""), LVCFMT_LEFT, 0);
}

void CFindBrowserExtensionsDlg::InsertListView(CString strItem, CString strDisplayName, CString strItemFullPath)
{
	int nIndex = m_ListCtrl.InsertItem(0, strItem);
	m_ListCtrl.SetItemText(nIndex, 1, strDisplayName);
	m_ListCtrl.SetItemText(nIndex, 2, strItemFullPath);
}

void CFindBrowserExtensionsDlg::ViewListInfo()
{
	m_ListCtrl.DeleteAllItems();
	
	if(m_pFinder)
	{
		std::vector<AddonInfo> addons = m_pFinder->getAddonInfo();
		for (int i = 0; i < addons.size(); i++)
		{
			InsertListView(addons[i].strItem, addons[i].strDisplayName, addons[i].strItemFullPath);		
		}
	}
}

CString CFindBrowserExtensionsDlg::GetFolderPath(CString strFolderPath)
{
	int nPos = strFolderPath.ReverseFind('\\');
	if (nPos > 0)
		return strFolderPath.Left(nPos);

	return strFolderPath;
}

void CFindBrowserExtensionsDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	CString strItem = m_ListCtrl.GetItemText(pNMItemActivate->iItem, 0);
	CString strDisplayName = m_ListCtrl.GetItemText(pNMItemActivate->iItem, 1);
	CString strItemFullPath = GetFolderPath(m_ListCtrl.GetItemText(pNMItemActivate->iItem, 2));
	ShellExecute(GetSafeHwnd(), NULL, strItemFullPath, NULL, NULL, SW_SHOW);

	*pResult = 0;
}

LRESULT CFindBrowserExtensionsDlg::OnFindExtensionDone(WPARAM wParam, LPARAM lParam)
{
	ViewListInfo();

	return 0;
}