#include "stdafx.h"
#include "mysqlconnect_test.h"
#include "mysqlconnect_testDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Cmysqlconnect_testDlg::Cmysqlconnect_testDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Cmysqlconnect_testDlg::IDD, pParent)
{
}

void Cmysqlconnect_testDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Cmysqlconnect_testDlg, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL Cmysqlconnect_testDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitDB();	

	return TRUE; 
}

void Cmysqlconnect_testDlg::OnPaint()
{
	CPaintDC dc(this);
}

BOOL Cmysqlconnect_testDlg::InitDB()
{
	CDBInfo db;
	if (db.Connect("127.0.0.1", "아이디", "비밀번호", "디비명", 3306)) 
	{
		vector<MYInfo> info = db.GetMyInfoList();
		db.ReadList(info);
		db.Disconnect();
		return TRUE;
	}
	return FALSE;
}