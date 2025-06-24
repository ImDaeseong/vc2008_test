#include "stdafx.h"
#include "gifPlayer_test.h"
#include "gifPlayer_testDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CgifPlayer_testDlg::CgifPlayer_testDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CgifPlayer_testDlg::IDD, pParent)
{
}

void CgifPlayer_testDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CgifPlayer_testDlg, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, &CgifPlayer_testDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CgifPlayer_testDlg::OnBnClickedButton2)
END_MESSAGE_MAP()

BOOL CgifPlayer_testDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
		
	m_GifStatic.SubclassDlgItem(IDC_STATIC_PICTURE, this);
	if(m_GifStatic.LoadGIF(_T("..\\1.gif")))
	{
		m_GifStatic.Start();
	}

	return TRUE;  
}

void CgifPlayer_testDlg::OnPaint()
{
	CPaintDC dc(this);
}

void CgifPlayer_testDlg::OnBnClickedButton1()
{
	if(m_GifStatic.LoadGIF(_T("..\\2.gif")))
	{
		m_GifStatic.Start();
	}	
}

void CgifPlayer_testDlg::OnBnClickedButton2()
{
	if(m_GifStatic.LoadGIF(_T("..\\1.gif")))
	{
		m_GifStatic.Start();
	}
}
