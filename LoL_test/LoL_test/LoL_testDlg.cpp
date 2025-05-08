#include "stdafx.h"
#include "LoL_test.h"
#include "LoL_testDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CLoL_testDlg::CLoL_testDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoL_testDlg::IDD, pParent)
{
	m_lolManager = NULL;
}

void CLoL_testDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLoL_testDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CLoL_testDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_lolManager = new CLoLManager();

	SetTimer(1, 10000, NULL);

	return TRUE;  
}

void CLoL_testDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if(m_lolManager)
	{
		delete m_lolManager;
		m_lolManager = NULL;
	}

	KillTimer(1);
}

void CLoL_testDlg::OnPaint()
{
	CPaintDC dc(this);
}

void CLoL_testDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == 1)
	{
		LoLDataInfo item = m_lolManager->GetGameDataInfo();
		CString strGameId = item.strGameId;
		CString strGameName = item.strGameName;
		CString strGameTag = item.strGameTag;	
		OutputDebugString(strGameId + CString("\r\n"));
	}

	CDialog::OnTimer(nIDEvent);
}
