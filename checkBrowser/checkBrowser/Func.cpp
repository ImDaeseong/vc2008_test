#include "StdAfx.h"
#include "Func.h"

CFunc::CFunc(void)
{
}

CFunc::~CFunc(void)
{
}

CString CFunc::GetProgramFilesFolder()
{
	TCHAR szPath[MAX_PATH];
	::SHGetSpecialFolderPath(NULL, szPath, CSIDL_PROGRAM_FILES, FALSE);
	return (CString)szPath;
}

CString CFunc::GetProgramFilesX86Folder()
{
    TCHAR szPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PROGRAM_FILESX86, NULL, 0, szPath)))
    {
        return CString(szPath);
    }
    return _T("");
}

CString CFunc::GetChromePath()
{
	HKEY hKey;
    
	LONG res = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\chrome.exe"),
        0, KEY_READ, &hKey);
    
	if (res == ERROR_SUCCESS) 
	{
        TCHAR path[MAX_PATH];
        DWORD pathSize = sizeof(path);
        res = RegQueryValueEx(hKey, NULL, NULL, NULL, (LPBYTE)path, &pathSize);
        RegCloseKey(hKey);

		if (res == ERROR_SUCCESS)
		{
            return CString(path);
        }
    }

	TCHAR browserPath[MAX_PATH];

    //Program Files 
    _tcscpy_s(browserPath, MAX_PATH, CFunc::GetProgramFilesFolder());
    _tcscat_s(browserPath, MAX_PATH, _T("\\Google\\Chrome\\Application\\chrome.exe"));
    if (GetFileAttributes(browserPath) != INVALID_FILE_ATTRIBUTES)
    {
        return CString(browserPath);
    }

    //Program Files (x86)
    _tcscpy_s(browserPath, MAX_PATH, CFunc::GetProgramFilesX86Folder());
    _tcscat_s(browserPath, MAX_PATH, _T("\\Google\\Chrome\\Application\\chrome.exe"));
    if (GetFileAttributes(browserPath) != INVALID_FILE_ATTRIBUTES)
    {
        return CString(browserPath);
    }

    return _T("");
}

CString CFunc::GetIePath()
{
	TCHAR browserPath[MAX_PATH];

    //Program Files 
    _tcscpy_s(browserPath, MAX_PATH, CFunc::GetProgramFilesFolder());
    _tcscat_s(browserPath, MAX_PATH, _T("\\Internet Explorer\\iexplore.exe"));
    if (_access(browserPath, 0) != -1) 
    {
        return CString(browserPath);
    }

    //Program Files (x86)
    _tcscpy_s(browserPath, MAX_PATH, CFunc::GetProgramFilesX86Folder());
    _tcscat_s(browserPath, MAX_PATH, _T("\\Internet Explorer\\iexplore.exe"));
    if (_access(browserPath, 0) != -1)
    {
        return CString(browserPath);
    }

    return _T("");
}

CString CFunc::GetEdgePath()
{
	HKEY hKey;
    LONG res = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
        _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\msedge.exe"),
        0, KEY_READ, &hKey);

    if (res == ERROR_SUCCESS)
    {
        TCHAR path[MAX_PATH];
        DWORD pathSize = sizeof(path);
        res = RegQueryValueEx(hKey, NULL, NULL, NULL, (LPBYTE)path, &pathSize);
        RegCloseKey(hKey);

        if (res == ERROR_SUCCESS)
        {
            if (GetFileAttributes(path) != INVALID_FILE_ATTRIBUTES)
            {
                return CString(path);
            }
        }
    }

    TCHAR browserPath[MAX_PATH];

    //Program Files 
    _tcscpy_s(browserPath, MAX_PATH, CFunc::GetProgramFilesFolder());
    _tcscat_s(browserPath, MAX_PATH, _T("\\Microsoft\\Edge\\Application\\msedge.exe"));
    if (GetFileAttributes(browserPath) != INVALID_FILE_ATTRIBUTES)
    {
        return CString(browserPath);
    }

    //Program Files (x86) 
    _tcscpy_s(browserPath, MAX_PATH, CFunc::GetProgramFilesX86Folder());
    _tcscat_s(browserPath, MAX_PATH, _T("\\Microsoft\\Edge\\Application\\msedge.exe"));
    if (GetFileAttributes(browserPath) != INVALID_FILE_ATTRIBUTES)
    {
        return CString(browserPath);
    }

    return _T("");
}

DWORD CFunc::ExecuteChromeBrowser(CString strUrl)
{
	CString strPath = CFunc::GetChromePath();
    if (strPath.IsEmpty())
		return 0;

	BOOL bNewWindow = TRUE;
	BOOL bMaximized = FALSE;

    CString strArgs = strUrl;
    if (bNewWindow) strArgs += _T(" --new-window");
    if (bMaximized) strArgs += _T(" --start-maximized");

    SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };
    sei.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FLAG_NO_UI;
    sei.lpVerb = _T("open");
    sei.lpFile = (LPCTSTR)strPath;
    sei.lpParameters = (LPCTSTR)strArgs;
    sei.nShow = SW_NORMAL;

    if (::ShellExecuteEx(&sei))
    {
        if (sei.hProcess != NULL)
        {
            DWORD dwPID = GetProcessId(sei.hProcess);
            CloseHandle(sei.hProcess);
            return dwPID;
        }
    }
    return 0;
}

DWORD CFunc::ExecuteIEBrowser(CString strUrl)
{
	CString strPath = GetIePath();
    if (strPath.IsEmpty())
		return 0;

	SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };
    sei.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FLAG_NO_UI;
    sei.lpVerb = _T("open");
    sei.lpFile = (LPCTSTR)strPath;
    sei.lpParameters = (LPCTSTR)strUrl;
    sei.nShow = SW_NORMAL;

    if (::ShellExecuteEx(&sei))
    {
        if (sei.hProcess != NULL)
        {
            DWORD dwPID = GetProcessId(sei.hProcess);
            CloseHandle(sei.hProcess);
            return dwPID;
        }
    }
    return 0;
}

DWORD CFunc::ExecuteEdgeBrowser(CString strUrl)
{
	CString strPath = CFunc::GetEdgePath();
    if (strPath.IsEmpty())
		return 0;

	BOOL bNewWindow = TRUE;
	BOOL bMaximized = FALSE;

    CString strArgs = strUrl;
    if (bNewWindow) strArgs += _T(" --new-window");
    if (bMaximized) strArgs += _T(" --start-maximized");

    SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };
    sei.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FLAG_NO_UI;
    sei.lpVerb = _T("open");
    sei.lpFile = (LPCTSTR)strPath;
    sei.lpParameters = (LPCTSTR)strArgs;
    sei.nShow = SW_NORMAL;

    if (::ShellExecuteEx(&sei))
    {
        if (sei.hProcess != NULL)
        {
            DWORD dwPID = GetProcessId(sei.hProcess);
            CloseHandle(sei.hProcess);
            return dwPID;
        }
    }
    return 0;
}