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
		/*
		LoLGameInfo item = m_lolManager->GetDataInfochat();
		CString strGameId = item.strGameId;
		CString strGameName = item.strGameName;
		CString strGameTag = item.strGameTag;	
		OutputDebugString(strGameId + CString("\r\n"));
		*/

		CString strItem1 = m_lolManager->GetDataInfochatString();		
		CString strItem2 = m_lolManager->GetDataInfosessionString();

		if (strItem1.IsEmpty()) strItem1 = _T("No chat data");
        if (strItem2.IsEmpty()) strItem2 = _T("No session data");

        SetDlgItemText(IDC_EDIT2, _T(""));
        SetDlgItemText(IDC_EDIT1, _T(""));

        SetDlgItemText(IDC_EDIT2, strItem1);
        SetDlgItemText(IDC_EDIT1, strItem2);
	}

	CDialog::OnTimer(nIDEvent);
}