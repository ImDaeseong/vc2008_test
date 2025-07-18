#include "stdafx.h"
#include "checkRedistInstall.h"
#include "checkRedistInstallDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CcheckRedistInstallDlg::CcheckRedistInstallDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CcheckRedistInstallDlg::IDD, pParent)
{
}

void CcheckRedistInstallDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CcheckRedistInstallDlg, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CcheckRedistInstallDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	/*
	//2022 재배포패키지
	if(isRedistPackage(TRUE))
	{
		AfxMessageBox(_T("2022 재배포패키지 64비트 이미설치"));	
	}
	else
	{
		InstallRedistPackage(TRUE);
	}

	if(isRedistPackage(FALSE))
	{
		AfxMessageBox(_T("2022 재배포패키지 32비트 이미설치"));	
	}
	else
	{
		InstallRedistPackage(FALSE);	
	}
	*/


	/*
	//Edge
	if(isEdgeBrowswer())
	{
		AfxMessageBox(_T("Edge 브라우저 이미설치"));	
	}
	else
	{
		//엣지브라우저
		InstallEdgeBrowswer(TRUE);
		InstallEdgeBrowswer(FALSE);
	}
	*/


	/*
	//.NET 4.8
	if(isInstallDotNetframework())
	{
		AfxMessageBox(_T(".NET 4.8 이상 이미설치"));
	}
	else
	{
		InstallDotNetframework();
	}
	*/


	///*
	//MariaDB 
	if(isMariaDB())
	{	
		//AfxMessageBox(_T("mariaDB 이미설치"));

		//MariaDB 서비스 생성
		bool bCreated = CreateServiceMariaDB();

		//MariaDB 서비스 시작
		bool bStart = StartServiceMariaDB();

		//MariaDB 루트 비밀번호 설정
		bool bPassword = CreateRootPasswordMariaDB();

		//Windows 방화벽에서 포트 허용
		bool bFirewall = ConfigureMariaDBFirewall(); 

	}
	else
	{
	    //mariaDB 설치
		InstallMariaDB();
	}
	//*/	
	
	return TRUE;  
}

void CcheckRedistInstallDlg::OnPaint()
{
	CPaintDC dc(this);
}

//MariaDB 서비스 생성
BOOL CcheckRedistInstallDlg::CreateServiceMariaDB()
{
	//mysqld --install "MariaDB" --defaults-file="C:\Program Files\MariaDB 10.13\data\my.ini"

	RegInfo reginfo;
	CString strPath = reginfo.GetMariaDBPath(_T("INSTALLDIR"));

	CString strBinPath;
	strBinPath.Format(_T("%s\\bin\\mysqld.exe"), strPath);

	CString strIniPath;
	strIniPath.Format(_T("%s\\data\\my.ini"), strPath);

	CString strParams;
    strParams.Format(_T("--install \"MariaDB\" --defaults-file=\"%s\""), strIniPath);
	
	SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };
    sei.fMask = SEE_MASK_NOCLOSEPROCESS;
    sei.lpVerb = _T("runas");           
    sei.lpFile = strBinPath;
    sei.lpParameters = strParams;
    sei.nShow = SW_HIDE;       

	if (!::ShellExecuteEx(&sei))
    {
        return FALSE;
    }

	BOOL bSuccess = FALSE;

    DWORD dwWaitResult = WaitForSingleObject(sei.hProcess, 30000); // 30초 대기
    if (dwWaitResult == WAIT_OBJECT_0)
    {
        DWORD dwExitCode = 0;
        if (GetExitCodeProcess(sei.hProcess, &dwExitCode))
        {
            if (dwExitCode == 0)
            {
                // MariaDB 서비스 설치 성공
                bSuccess = TRUE;
            }
        }
    }

    CloseHandle(sei.hProcess);
    return bSuccess;
}

//MariaDB 서비스 시작
BOOL CcheckRedistInstallDlg::StartServiceMariaDB()
{     
	CString strCommand = _T("cmd.exe");
    CString strParams = _T("/c net start MariaDB");

    SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };
    sei.fMask = SEE_MASK_NOCLOSEPROCESS;
    sei.lpVerb = _T("runas");
    sei.lpFile = strCommand;
    sei.lpParameters = strParams;
    sei.nShow = SW_HIDE;

	if (!::ShellExecuteEx(&sei))
	{
		return FALSE;
	}

	DWORD dwWaitResult = WaitForSingleObject(sei.hProcess, INFINITE);
    DWORD dwExitCode = 0;

	BOOL bSuccess = FALSE;

    if (dwWaitResult == WAIT_OBJECT_0)
    {
        if (GetExitCodeProcess(sei.hProcess, &dwExitCode))
        {
            if (dwExitCode == 0 || dwExitCode == 1056)
            {
				 //정상적으로 시작되었거나 이미 실행 중
                bSuccess = TRUE; 
            }
        }
    }

    CloseHandle(sei.hProcess);
    return bSuccess;
}

BOOL CcheckRedistInstallDlg::CreateRootPasswordMariaDB()
{
	//mysqladmin.exe -u root password "MyStrongNewPassword"

	RegInfo reginfo;
	CString strPath = reginfo.GetMariaDBPath(_T("INSTALLDIR"));

	CString strBinPath;
	strBinPath.Format(_T("%s\\bin\\mysqladmin.exe"), strPath);

	CString strRootPassword = _T("1234");
	
	CString strParams;
    strParams.Format(_T("-u root password \"%s\""), strRootPassword);
	
	SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };
    sei.fMask = SEE_MASK_NOCLOSEPROCESS;
    sei.lpVerb = _T("runas");           
    sei.lpFile = strBinPath;
    sei.lpParameters = strParams;
    sei.nShow = SW_HIDE;       
	 
	if (!::ShellExecuteEx(&sei))
    {
        return FALSE;
    }
	 
	BOOL bSuccess = FALSE;

    DWORD dwWaitResult = WaitForSingleObject(sei.hProcess, 30000); // 30초 대기
    if (dwWaitResult == WAIT_OBJECT_0)
    {
        DWORD dwExitCode = 0;
        if (GetExitCodeProcess(sei.hProcess, &dwExitCode))
        {
            if (dwExitCode == 0)
            {
                // 성공
                bSuccess = TRUE;
            }
        }
    }

    CloseHandle(sei.hProcess);
    return bSuccess;
}

BOOL CcheckRedistInstallDlg::ConfigureMariaDBFirewall() 
{
	//3306 포트 허용
	int nPort = 3306;

    CString strCommand = _T("netsh");
    CString strRuleName;
    strRuleName.Format(_T("\"MariaDB Port %d\""), nPort); 

    CString strParams;
    strParams.Format(_T("advfirewall firewall add rule name=%s dir=in action=allow protocol=TCP localport=%d"), strRuleName, nPort);

    SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };
    sei.fMask = SEE_MASK_NOCLOSEPROCESS;
    sei.lpVerb = _T("runas");
    sei.lpFile = strCommand;
    sei.lpParameters = strParams;
    sei.nShow = SW_HIDE;
	
    if (!::ShellExecuteEx(&sei))
    {
        return FALSE;
    }

	BOOL bSuccess = FALSE;

    DWORD dwWaitResult = WaitForSingleObject(sei.hProcess, 30000); // 30초 대기
    if (dwWaitResult == WAIT_OBJECT_0)
    {
        DWORD dwExitCode = 0;
        if (GetExitCodeProcess(sei.hProcess, &dwExitCode))
        {
            if (dwExitCode == 0)
            {
				//MariaDB 방화벽 규칙 추가
                bSuccess = TRUE;
            }
        }
    }

    CloseHandle(sei.hProcess);
    return bSuccess;
}

//MariaDB 설치
BOOL CcheckRedistInstallDlg::InstallMariaDB()
{
	CString strSetupFile = _T("D:\\setupfiles\\mariadb-10.11.13-winx64.msi");

	CString strParam;
	strParam.Format(_T("/i \"%s\" /qn /norestart"), strSetupFile);

	HINSTANCE hInstance = ::ShellExecute(NULL, _T("open"), _T("msiexec.exe"), strParam, NULL, SW_HIDE);
	if ((INT_PTR)hInstance <= 32)
		return FALSE;
 	
	return TRUE;
}

//MariaDB 패키지 설치 여부 체크
BOOL CcheckRedistInstallDlg::isMariaDB()
{
	RegInfo reginfo;
	
	bool bIs64 = reginfo.GetMariaDBPackageInstalled(_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall"), _T("{65376983-9602-406B-881A-3F153BDC394F}"));
	bool bIs32 = reginfo.GetMariaDBPackageInstalled(_T("SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall"), _T("{65376983-9602-406B-881A-3F153BDC394F}"));
	if(bIs64 || bIs32)			
		return TRUE;

	return FALSE;
}

// Edge 브라우저 설치 여부 체크
BOOL CcheckRedistInstallDlg::isEdgeBrowswer()
{
	CString strDisplayVersion = _T("");

	RegInfo reginfo;
	strDisplayVersion = reginfo.GetEdgeVersion(_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Microsoft EdgeWebView"), _T("DisplayVersion"));
   	if (!strDisplayVersion.IsEmpty())
	{
		return TRUE;
	}
	
	strDisplayVersion = reginfo.GetEdgeVersion(_T("SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Microsoft EdgeWebView"), _T("DisplayVersion"));
	if (!strDisplayVersion.IsEmpty())
	{
		return TRUE;
	}

	return FALSE;
}

// Microsoft Edge WebView2 런타임 설치
BOOL CcheckRedistInstallDlg::InstallEdgeBrowswer(bool isX64)
{
	if(isX64)
	{
		CString strSetupFile;
		strSetupFile.Format(_T("..\\setupfiles\\MicrosoftEdgeWebView2RuntimeInstallerX64.exe"));

		CString strSetupParam = "";
		strSetupParam.Format(_T("/silent /install"));

		HINSTANCE hInstance = ::ShellExecute(NULL, _T("open"), strSetupFile, strSetupParam, NULL, SW_HIDE);
		if (hInstance <= (HINSTANCE)32)
			return FALSE;

		return TRUE;
	}
	else
	{
		CString strSetupFile;
		strSetupFile.Format(_T("..\\setupfiles\\MicrosoftEdgeWebView2RuntimeInstallerX86.exe"));

		CString strSetupParam = "";
		strSetupParam.Format(_T("/silent /install"));

		HINSTANCE hInstance = ::ShellExecute(NULL, _T("open"), strSetupFile, strSetupParam, NULL, SW_HIDE);
		if (hInstance <= (HINSTANCE)32)
			return FALSE;

		return TRUE;
	}

	return TRUE;
}

// 2022 재배포패키지 설치 여부 체크
BOOL CcheckRedistInstallDlg::isRedistPackage(bool isX64)
{
	/*
	64비트
	{130A991B-6E86-4D62-86F4-656E6C9DA867}
	Microsoft Visual C++ 2022 X64 Minimum Runtime - 14.44.35208

	{7E35F76C-5801-4A4A-817C-66AF46D3E5AD}
	Microsoft Visual C++ 2022 X64 Additional Runtime - 14.44.35208

	32비트
	{491C67BA-2F2F-4A90-B9DD-4C76BFDBEA02}
	Microsoft Visual C++ 2022 X86 Minimum Runtime - 14.44.35208

	{5A76FFAE-36C5-4648-80BD-4BB5B6E971F0}
	Microsoft Visual C++ 2022 X86 Additional Runtime - 14.44.35208
	*/

	if(isX64)
	{
		RegInfo reginfo;
		bool bIs64Installed1 = reginfo.GetRedistPackageInstalled(_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall"), _T("{130A991B-6E86-4D62-86F4-656E6C9DA867}"));
		bool bIs64Installed2 = reginfo.GetRedistPackageInstalled(_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall"), _T("{7E35F76C-5801-4A4A-817C-66AF46D3E5AD}"));
		
		bool bIs64Installed3 = reginfo.GetRedistPackageInstalled(_T("SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall"), _T("{130A991B-6E86-4D62-86F4-656E6C9DA867}"));
		bool bIs64Installed4 = reginfo.GetRedistPackageInstalled(_T("SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall"), _T("{7E35F76C-5801-4A4A-817C-66AF46D3E5AD}"));
		
		if( (bIs64Installed1 && bIs64Installed2) || (bIs64Installed3 && bIs64Installed4) )
			return TRUE;
	}
	else
	{
		RegInfo reginfo;

		bool bIs32Installed1 = reginfo.GetRedistPackageInstalled(_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall"), _T("{491C67BA-2F2F-4A90-B9DD-4C76BFDBEA02}"));
		bool bIs32Installed2 = reginfo.GetRedistPackageInstalled(_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall"), _T("{5A76FFAE-36C5-4648-80BD-4BB5B6E971F0}"));
		
		bool bIs32Installed3 = reginfo.GetRedistPackageInstalled(_T("SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall"), _T("{491C67BA-2F2F-4A90-B9DD-4C76BFDBEA02}"));
		bool bIs32Installed4 = reginfo.GetRedistPackageInstalled(_T("SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall"), _T("{5A76FFAE-36C5-4648-80BD-4BB5B6E971F0}"));
	
		if ( (bIs32Installed1 && bIs32Installed2) || (bIs32Installed3 && bIs32Installed4) )
			return TRUE;	
	}

	return FALSE;
}

// 2022 재배포패키지 설치
BOOL CcheckRedistInstallDlg::InstallRedistPackage(bool isX64)
{
	if(isX64)
	{
		CString strSetupFile;
		strSetupFile.Format(_T("..\\setupfiles\\VC_redist.x64.exe"));

		CString strSetupParam = "";
		strSetupParam.Format(_T("/install /quiet /norestart"));

		HINSTANCE hInstance = ::ShellExecute(NULL, _T("open"), strSetupFile, strSetupParam, NULL, SW_HIDE);
		if (hInstance <= (HINSTANCE)32)
			return FALSE;

		return TRUE;
	}
	else
	{
		CString strSetupFile;
		strSetupFile.Format(_T("..\\setupfiles\\VC_redist.x86.exe"));

		CString strSetupParam = "";
		strSetupParam.Format(_T("/install /quiet /norestart"));

		HINSTANCE hInstance = ::ShellExecute(NULL, _T("open"), strSetupFile, strSetupParam, NULL, SW_HIDE);
		if (hInstance <= (HINSTANCE)32)
			return FALSE;

		return TRUE;
	}
}

// .NET 4.8 설치 여부 체크
BOOL CcheckRedistInstallDlg::isInstallDotNetframework()
{
	DWORD dwVersion = 0;

	RegInfo reginfo;
	dwVersion = reginfo.GetDotNetFrameworkVersion(_T("SOFTWARE\\Microsoft\\NET Framework Setup\\NDP\\v4\\Full"), _T("Release"));
   	if (dwVersion >= 528040) 
	{
		return TRUE;
	}
	
	dwVersion = reginfo.GetDotNetFrameworkVersion(_T("SOFTWARE\\WOW6432Node\\Microsoft\\NET Framework Setup\\NDP\\v4\\Full"), _T("Release"));
	if (dwVersion >= 528040) 
	{
		return TRUE;
	}

	return FALSE;
}

// .NET 4.8 silent mode 설치
BOOL CcheckRedistInstallDlg::InstallDotNetframework()
{
	CString strSetupFile;
	strSetupFile.Format(_T("..\\setupfiles\\NDP48-x86-x64-AllOS-ENU.exe"));

	CString strSetupParam = "";
	strSetupParam.Format(_T("/q /norestart"));

	HINSTANCE hInstance = ::ShellExecute(NULL, _T("open"), strSetupFile, strSetupParam, NULL, SW_HIDE);
	if (hInstance <= (HINSTANCE)32)
		return FALSE;

	return TRUE;
}