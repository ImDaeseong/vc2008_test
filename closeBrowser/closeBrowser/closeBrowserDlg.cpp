#include "stdafx.h"
#include "closeBrowser.h"
#include "closeBrowserDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


typedef struct _FIND_WINDOW_PARAM {
    DWORD dwProcessID;
    HWND hwnd;
} FIND_WINDOW_PARAM;

BOOL CALLBACK FindWindowFromPidProc(HWND hwnd, LPARAM lParam)
{
    DWORD dwProcessID = 0;
    FIND_WINDOW_PARAM* findParam = (FIND_WINDOW_PARAM*)lParam;
    GetWindowThreadProcessId(hwnd, &dwProcessID);

    if (findParam->dwProcessID == dwProcessID)
    {
        if (::IsWindowVisible(hwnd))
        {
            findParam->hwnd = hwnd;
            return FALSE;
        }

        HWND owner = ::GetWindow(hwnd, GW_OWNER);
        if (owner && ::IsWindowVisible(owner))
        {
            findParam->hwnd = owner;
            return FALSE;
        }
    }

    return TRUE; 
}

HWND FindWindowFromProcessId(DWORD dwProcessID)
{
    FIND_WINDOW_PARAM param;
    ZeroMemory(&param, sizeof(param));
    param.dwProcessID = dwProcessID;

    ::EnumWindows(FindWindowFromPidProc, (LPARAM)&param);
    return param.hwnd;
}

CString GetExecuteFileName(DWORD processID)
{
    CString strFileName;
    PROCESSENTRY32 pe32;
    ZeroMemory(&pe32, sizeof(pe32));
    pe32.dwSize = sizeof(PROCESSENTRY32);

    HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
        return strFileName;

    if (::Process32First(hSnapshot, &pe32)) 
	{
        do 
		{
            if (pe32.th32ProcessID == processID) 
			{
                strFileName = pe32.szExeFile;
                break;
            }

        } while (::Process32Next(hSnapshot, &pe32));
    }

    ::CloseHandle(hSnapshot);
    return strFileName;
}


CcloseBrowserDlg::CcloseBrowserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CcloseBrowserDlg::IDD, pParent)
{
}

void CcloseBrowserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CcloseBrowserDlg, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, &CcloseBrowserDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CcloseBrowserDlg::OnBnClickedButton2)
END_MESSAGE_MAP()

BOOL CcloseBrowserDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE; 
}

void CcloseBrowserDlg::OnPaint()
{
	CPaintDC dc(this);
}

void CcloseBrowserDlg::OnBnClickedButton1()
{
	CloseAllBrowsers(TRUE);
}

void CcloseBrowserDlg::OnBnClickedButton2()
{
	CloseAllBrowsers(FALSE);
}

BOOL CcloseBrowserDlg::CloseWebBrowserTerminate(const CString& strExeName)
{
    BOOL bResult = FALSE;

    HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
        return FALSE;

    PROCESSENTRY32 proc;
    ZeroMemory(&proc, sizeof(proc));
    proc.dwSize = sizeof(PROCESSENTRY32);

    if (!::Process32First(hSnapshot, &proc)) 
	{
        ::CloseHandle(hSnapshot);
        return FALSE;
    }

    do 
	{
        if (strExeName.CompareNoCase(proc.szExeFile) == 0)
        {
            HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, proc.th32ProcessID);
            if (hProcess)
            {
                if (::TerminateProcess(hProcess, 0))
                {
                    bResult = TRUE;
                }
                ::CloseHandle(hProcess);
            }
        }

    } while (::Process32Next(hSnapshot, &proc));

    ::CloseHandle(hSnapshot);
    return bResult;
}

BOOL CcloseBrowserDlg::CloseWebBrowserClose(const CString& strExeName)
{
    BOOL bResult = FALSE;

    HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
        return FALSE;

    PROCESSENTRY32 proc;
    ZeroMemory(&proc, sizeof(proc));
    proc.dwSize = sizeof(PROCESSENTRY32);

    if (!::Process32First(hSnapshot, &proc)) 
	{
        ::CloseHandle(hSnapshot);
        return FALSE;
    }

    do {
        
		if (strExeName.CompareNoCase(proc.szExeFile) == 0)
        {
            HWND hWnd = FindWindowFromProcessId(proc.th32ProcessID);
            if (hWnd)
            {
                ::PostMessage(hWnd, WM_CLOSE, 0, 0);
                bResult = TRUE;
            }
        }

    } while (::Process32Next(hSnapshot, &proc));

    ::CloseHandle(hSnapshot);
    return bResult;
}

void CcloseBrowserDlg::CloseAllBrowsers(BOOL bHwnd)
{
    static LPCTSTR browsers[] =
    {
        _T("chrome.exe"),
        _T("msedge.exe"),
        _T("firefox.exe"),
        _T("iexplore.exe"),
        _T("microsoftedge.exe"),
        _T("microsoftedgecp.exe"),
        _T("applicationframehost.exe"),
        _T("whale.exe")
    };

    int nCount = sizeof(browsers) / sizeof(browsers[0]);

    for (int i = 0; i < nCount; ++i)
    {
        if (bHwnd)
		{
            CloseWebBrowserClose(browsers[i]);
		}
        else
		{
            CloseWebBrowserTerminate(browsers[i]);
		}
    }
}