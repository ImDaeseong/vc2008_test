#include "StdAfx.h"
#include "RegInfo.h"

RegInfo::RegInfo()
{
}

RegInfo::~RegInfo()
{
}

DWORD RegInfo::GetDotNetFrameworkVersion(LPCTSTR pszSubKey, LPCTSTR pszValueName)
{
	CRegKey regKey;
    DWORD dwRelease = 0;
    LONG lResult = regKey.Open(HKEY_LOCAL_MACHINE, pszSubKey, KEY_READ | KEY_WOW64_64KEY);
    
    if (lResult == ERROR_SUCCESS)
    {
        DWORD dwType = REG_DWORD;
        ULONG nBytes = sizeof(DWORD);
        lResult = regKey.QueryValue(dwRelease, pszValueName);
        regKey.Close();
    }
    return dwRelease;
}

bool RegInfo::GetRedistPackageInstalled(LPCTSTR pszBaseKey, LPCTSTR pszSubKey)
{
	CString strFullKey;
    strFullKey.Format(_T("%s\\%s"), pszBaseKey, pszSubKey);
	 
	CRegKey regKey;
    LONG lResult = regKey.Open(HKEY_LOCAL_MACHINE, strFullKey, KEY_READ | KEY_WOW64_64KEY);
	if (lResult == ERROR_SUCCESS)
    {
        regKey.Close();
        return true;
    }
    return false;
}

CString RegInfo::GetEdgeVersion(LPCTSTR pszSubKey, LPCTSTR pszValueName)
{
	CRegKey regKey;
    CString strValue = _T("");
    LONG lResult = regKey.Open(HKEY_LOCAL_MACHINE, pszSubKey, KEY_READ | KEY_WOW64_64KEY);

	if (lResult == ERROR_SUCCESS)
	{
		TCHAR szBuffer[256] = { 0 };
        ULONG nChars = sizeof(szBuffer) / sizeof(TCHAR);

		lResult = regKey.QueryStringValue(pszValueName, szBuffer, &nChars);
        if (lResult == ERROR_SUCCESS)
        {
            strValue = szBuffer;
        }
        regKey.Close();	
	}
	return strValue;
}

bool RegInfo::GetMariaDBPackageInstalled(LPCTSTR pszBaseKey, LPCTSTR pszSubKey)
{
	CString strFullKey;
    strFullKey.Format(_T("%s\\%s"), pszBaseKey, pszSubKey);
	 
	CRegKey regKey;
    LONG lResult = regKey.Open(HKEY_LOCAL_MACHINE, strFullKey, KEY_READ | KEY_WOW64_64KEY);
	if (lResult == ERROR_SUCCESS)
    {
        regKey.Close();
        return true;
    }
    return false;
}

CString RegInfo::GetMariaDBPath(LPCTSTR pszValueName)
{
	CString strFullKey;
    strFullKey.Format(_T("SOFTWARE\\MariaDB 10.11 (x64)"));

	CRegKey regKey;
    CString strValue = _T("");
    LONG lResult = regKey.Open(HKEY_LOCAL_MACHINE, strFullKey, KEY_READ | KEY_WOW64_64KEY);

	if (lResult == ERROR_SUCCESS)
	{
		TCHAR szBuffer[256] = { 0 };
        ULONG nChars = sizeof(szBuffer) / sizeof(TCHAR);

		lResult = regKey.QueryStringValue(pszValueName, szBuffer, &nChars);
        if (lResult == ERROR_SUCCESS)
        {
            strValue = szBuffer;
        }
        regKey.Close();	
	}
	return strValue;
}