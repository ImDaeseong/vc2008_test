#include "StdAfx.h"
#include "RegInfo.h"

RegInfo::RegInfo(void)
{
}

RegInfo::~RegInfo(void)
{
}

CString RegInfo::GetAppDataPath()
{
    TCHAR tPath[MAX_PATH];
    ULONG size = MAX_PATH; 

    CString strPath;
    CRegKey regKey;

    if (regKey.Open(HKEY_CURRENT_USER, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"), KEY_READ) != ERROR_SUCCESS)
    {
        return _T("");
    }

    if (regKey.QueryStringValue(_T("AppData"), tPath, &size) == ERROR_SUCCESS)
    {
        strPath = tPath;
    }
    regKey.Close();

    return strPath;
}

CString RegInfo::GetLocalAppDataPath()
{
	TCHAR tPath[MAX_PATH];
	ULONG size = MAX_PATH; 

	CString strPath;
	CRegKey regKey;
	
	if (regKey.Open(HKEY_CURRENT_USER, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"), KEY_READ) != ERROR_SUCCESS)
	{
		return _T("");
	}

	if (regKey.QueryStringValue(_T("Local AppData"), tPath, &size) == ERROR_SUCCESS)
	{
		strPath = tPath;
	}
	regKey.Close();

	return strPath;
}