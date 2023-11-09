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
}

BEGIN_MESSAGE_MAP(CFindBrowserExtensionsDlg, CDialog)
	ON_WM_PAINT()	
	ON_WM_DESTROY()
END_MESSAGE_MAP()

BOOL CFindBrowserExtensionsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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