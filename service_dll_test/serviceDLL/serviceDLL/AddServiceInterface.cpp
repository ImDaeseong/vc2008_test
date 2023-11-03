#include "StdAfx.h"


#define MAX_WAIT_TIME	5000

LPCTSTR g_chServiceName = _T("myservice");
LPCTSTR g_chDisplay = _T("myservice DLL");


CString GetModulePath()
{
    TCHAR szTemp[MAX_PATH];
    TCHAR *pszRealDirEnd;
    CString strModulePath;

    GetModuleFileName(NULL, szTemp, MAX_PATH);
    pszRealDirEnd = _tcsrchr(szTemp, _T('\\'));
    if (pszRealDirEnd != NULL)
        *pszRealDirEnd = _T('\0');

    strModulePath = (CString)szTemp;

    return strModulePath;
}

BOOL InstallService(LPSTR szFilename)
{
	CString szPath;
	szPath.Format(_T("%s\\%S"), GetModulePath(), szFilename); // %S 대문자 사용
	//::MessageBox(NULL, szPath, NULL, MB_OK);

	SC_HANDLE hScm = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
	if (hScm == NULL)
		return FALSE;

	SC_HANDLE hSrv = CreateService(
						hScm,
						g_chServiceName,
						g_chDisplay,
						SERVICE_ALL_ACCESS,
						SERVICE_WIN32_OWN_PROCESS,
						SERVICE_AUTO_START, //SERVICE_AUTO_START,자동 //SERVICE_DEMAND_START,수동
						SERVICE_ERROR_IGNORE,
						szPath,
						NULL,
						NULL,
						NULL,
						NULL,
						NULL);

	if (hSrv == NULL)
	{
		CloseServiceHandle(hScm);
		return FALSE;
	}

	CloseServiceHandle(hSrv);
	CloseServiceHandle(hScm);


	return TRUE;
}

BOOL UninstallService()
{
	SERVICE_STATUS status;
	DWORD dwTickStart = 0;
	DWORD dwTickCurrent = 0;
	
	SC_HANDLE hServiceManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(hServiceManager == NULL)
		return FALSE;
	
	SC_HANDLE hService = OpenService(hServiceManager, g_chServiceName, SERVICE_ALL_ACCESS);
	if(hService == NULL)
	{
		CloseServiceHandle(hServiceManager);
		return FALSE;
	}

	memset(&status, 0x00, sizeof(SERVICE_STATUS));

	BOOL bRes = QueryServiceStatus(hService, &status);
	if(bRes)
	{
		if(status.dwCurrentState != SERVICE_STOPPED)
			bRes = ControlService(hService, SERVICE_CONTROL_STOP, &status);
	}

	dwTickStart = GetTickCount();	
	while(bRes != FALSE && status.dwCurrentState != SERVICE_STOPPED)
	{		
		dwTickCurrent = GetTickCount();
		
		if(dwTickCurrent < dwTickStart) 
			dwTickStart = dwTickCurrent;
	
		if(dwTickCurrent - dwTickStart >= MAX_WAIT_TIME)
			break;

		Sleep(100);

		bRes = QueryServiceStatus(hService, &status);
	}

	bRes = DeleteService(hService);

	CloseServiceHandle(hService);
	CloseServiceHandle(hServiceManager);

	return bRes;
}

BOOL StartService()
{
	SERVICE_STATUS status;
	DWORD dwTickStart = 0;
	DWORD dwTickCurrent = 0;
	
	SC_HANDLE hServiceManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(hServiceManager == NULL)
		return FALSE;
	
	SC_HANDLE hService = OpenService(hServiceManager, g_chServiceName, SERVICE_ALL_ACCESS);
	if(hService == NULL)
	{
		CloseServiceHandle(hServiceManager);
		return FALSE;
	}

	memset(&status, 0x00, sizeof(SERVICE_STATUS));

	BOOL bRes = QueryServiceStatus(hService, &status);
	if(bRes)
	{
		if(status.dwCurrentState == SERVICE_STOPPED)
			bRes = ::StartService(hService, 0, NULL);
	}

	CloseServiceHandle(hService);
	CloseServiceHandle(hServiceManager);

	return bRes;
}

BOOL StopService()
{
	SERVICE_STATUS status;
	DWORD dwTickStart = 0;
	DWORD dwTickCurrent = 0;
	
	SC_HANDLE hServiceManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(hServiceManager == NULL)
		return FALSE;
	
	SC_HANDLE hService = OpenService(hServiceManager, g_chServiceName, SERVICE_ALL_ACCESS);
	if(hService == NULL)
	{
		CloseServiceHandle(hServiceManager);
		return FALSE;
	}

	memset(&status, 0x00, sizeof(SERVICE_STATUS));

	BOOL bRes = QueryServiceStatus(hService, &status);
	if(bRes)
	{
		if(status.dwCurrentState != SERVICE_STOPPED)
			bRes = ControlService(hService, SERVICE_CONTROL_STOP, &status);
	}

	dwTickStart = GetTickCount();	
	while(bRes != FALSE && status.dwCurrentState != SERVICE_STOPPED)
	{		
		dwTickCurrent = GetTickCount();
		
		if(dwTickCurrent < dwTickStart) 
			dwTickStart = dwTickCurrent;
	
		if(dwTickCurrent - dwTickStart >= MAX_WAIT_TIME)
			break;

		Sleep(100);

		bRes = QueryServiceStatus(hService, &status);
	}

	CloseServiceHandle(hService);
	CloseServiceHandle(hServiceManager);

	if(status.dwCurrentState != SERVICE_STOPPED)
		return FALSE;

	return TRUE;
}

BOOL IsInstallService()
{
	SC_HANDLE hServiceManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hServiceManager == NULL)
		return FALSE;

	SC_HANDLE hService = OpenService(hServiceManager, g_chServiceName, SERVICE_QUERY_STATUS);
	if (hService == NULL)
	{
		CloseServiceHandle(hServiceManager);
		return FALSE;
	}

	CloseServiceHandle(hService);
	CloseServiceHandle(hServiceManager);

	return TRUE;
}


extern "C"
{

	__declspec(dllexport) BOOL ApiInstallService(LPSTR szFilename)
	{
	
		return InstallService(szFilename);
	}

	__declspec(dllexport) BOOL ApiUninstallService()
	{
	
		return UninstallService();
	}

	__declspec(dllexport) BOOL ApiStartService()
	{
		if(IsInstallService())
		{
			return StartService();		
		}
	
		return false;
	}


	__declspec(dllexport) BOOL ApiStopService()
	{
	
		return StopService();
	}

};