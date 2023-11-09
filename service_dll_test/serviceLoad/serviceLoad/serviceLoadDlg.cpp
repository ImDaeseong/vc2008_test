#include "stdafx.h"
#include "serviceLoad.h"
#include "serviceLoadDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CserviceLoadDlg::CserviceLoadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CserviceLoadDlg::IDD, pParent)
{
	m_hDll = NULL;
}

void CserviceLoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CserviceLoadDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, &CserviceLoadDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CserviceLoadDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CserviceLoadDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CserviceLoadDlg::OnBnClickedButton4)
END_MESSAGE_MAP()

BOOL CserviceLoadDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_hDll = ::LoadLibrary(L"serviceDLL.dll");
	if(m_hDll == NULL)
	{
		AfxMessageBox(_T("load err"), MB_ICONERROR);
		return FALSE;
	}

	return TRUE;  
}


void CserviceLoadDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if(m_hDll)
	{
		FreeLibrary(m_hDll);
		m_hDll = NULL;
	}
}

void CserviceLoadDlg::OnPaint()
{
	CPaintDC dc(this);
}


//���� ��ġ
void CserviceLoadDlg::OnBnClickedButton1()
{

	PFN_INSTALLSERVICE pfnInstallService = (PFN_INSTALLSERVICE)::GetProcAddress(m_hDll, "ApiInstallService");
	
	if (pfnInstallService)
	{
		//myservice.exe ���� ���α׷��� ��ġ
		if (pfnInstallService("myservice.exe"))
		{
			AfxMessageBox(_T("ApiInstallService ����"), MB_ICONINFORMATION);
		}
		else
		{
			AfxMessageBox(_T("ApiInstallService ����"), MB_ICONINFORMATION);
		}
	}
	else
	{
		AfxMessageBox(_T("ApiInstallService err"), MB_ICONERROR);
	}
}

//���� ����
void CserviceLoadDlg::OnBnClickedButton2()
{

	PFN_UNINSTALLSERVICE pfnUninstallService = (PFN_UNINSTALLSERVICE)::GetProcAddress(m_hDll, "ApiUninstallService");
	
	if (pfnUninstallService)
	{
		if (pfnUninstallService())
		{
			AfxMessageBox(_T("ApiUninstallService ����"), MB_ICONINFORMATION);
		}
		else
		{
			AfxMessageBox(_T("ApiUninstallService ����"), MB_ICONINFORMATION);
		}
	}
	else
	{
		AfxMessageBox(_T("ApiUninstallService err"), MB_ICONERROR);
	}
}

//���� ����
void CserviceLoadDlg::OnBnClickedButton3()
{

	PFN_STARTSERVICE pfnStartService = (PFN_STARTSERVICE)::GetProcAddress(m_hDll, "ApiStartService");
	
	if (pfnStartService)
	{
		if (pfnStartService())
		{
			AfxMessageBox(_T("ApiStartService ����"), MB_ICONINFORMATION);
		}
		else
		{
			AfxMessageBox(_T("ApiStartService ����"), MB_ICONINFORMATION);
		}
	}
	else
	{
		AfxMessageBox(_T("ApiStartService err"), MB_ICONERROR);
	}
}

//���� ����
void CserviceLoadDlg::OnBnClickedButton4()
{

	PFN_STOPSERVICE pfnStopService = (PFN_STOPSERVICE)::GetProcAddress(m_hDll, "ApiStopService");
	
	if (pfnStopService)
	{
		if (pfnStopService())
		{
			AfxMessageBox(_T("ApiStopService ����"), MB_ICONINFORMATION);
		}
		else
		{
			AfxMessageBox(_T("ApiStopService ����"), MB_ICONINFORMATION);
		}
	}
	else
	{
		AfxMessageBox(_T("ApiStopService err"), MB_ICONERROR);
	}
}
