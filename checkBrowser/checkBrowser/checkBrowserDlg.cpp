#include "stdafx.h"
#include "checkBrowser.h"
#include "checkBrowserDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CcheckBrowserDlg::CcheckBrowserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CcheckBrowserDlg::IDD, pParent)
{
}

void CcheckBrowserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CcheckBrowserDlg, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, &CcheckBrowserDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CcheckBrowserDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CcheckBrowserDlg::OnBnClickedButton3)
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CcheckBrowserDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_timer = SetTimer(1, 5000, NULL);

	return TRUE;  
}

void CcheckBrowserDlg::OnPaint()
{
	CPaintDC dc(this);
}

void CcheckBrowserDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if (m_timer != 0)
    {
        KillTimer(m_timer);
        m_timer = 0;
    }
}

void CcheckBrowserDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
    {
		FindBrowserPIDs();
		FindBrowserPIDsTime();

		DWORD dwID = FindBrowserPIDsLast();
		CString msg;
        msg.Format(_T("Found: (PID: %u)\n"), dwID);
		OutputDebugString(msg);
    }

	CDialog::OnTimer(nIDEvent);
}

//브라우저 전체 PID
void CcheckBrowserDlg::FindBrowserPIDs()
{
	CString browsers[] = { _T("chrome.exe"), _T("msedge.exe"), _T("iexplore.exe") };

	PROCESSENTRY32 processInfo = {0};
    processInfo.dwSize = sizeof(PROCESSENTRY32);

    HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (processesSnapshot == INVALID_HANDLE_VALUE)
        return;

	if (Process32First(processesSnapshot, &processInfo))
    {
        do
        {
            for (int i = 0; i < _countof(browsers); ++i)
            {
                if (browsers[i].CompareNoCase(processInfo.szExeFile) == 0)
                {
                    CString msg;
                    msg.Format(_T("Found: %s (PID: %u)\n"), processInfo.szExeFile, processInfo.th32ProcessID);
                    OutputDebugString(msg);
                    break; 
                }
            }

        } while (Process32Next(processesSnapshot, &processInfo));
    }

    CloseHandle(processesSnapshot);
}

//브라우저 전체 PID, 브라우저 생성시간
void CcheckBrowserDlg::FindBrowserPIDsTime()
{
    CString browsers[] = { _T("chrome.exe"), _T("msedge.exe"), _T("iexplore.exe") };

    PROCESSENTRY32 processInfo = {0};
    processInfo.dwSize = sizeof(PROCESSENTRY32);

    HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (processesSnapshot == INVALID_HANDLE_VALUE)
        return;

    if (Process32First(processesSnapshot, &processInfo))
    {
        do
        {
            for (int i = 0; i < _countof(browsers); ++i)
            {
                if (browsers[i].CompareNoCase(processInfo.szExeFile) == 0)
                {
                    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, processInfo.th32ProcessID);
                    if (hProcess != NULL)
                    {
                        FILETIME creationTime, exitTime, kernelTime, userTime;
                        if (GetProcessTimes(hProcess, &creationTime, &exitTime, &kernelTime, &userTime))
                        {
                            SYSTEMTIME stUTC, stLocal;
                            FileTimeToSystemTime(&creationTime, &stUTC);
                            SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

                            CString msg;
                            msg.Format(_T("Found: %s (PID: %u) | 생성 시간: %04d-%02d-%02d %02d:%02d:%02d\n"),
                                       processInfo.szExeFile,
                                       processInfo.th32ProcessID,
                                       stLocal.wYear, stLocal.wMonth, stLocal.wDay,
                                       stLocal.wHour, stLocal.wMinute, stLocal.wSecond);

                            OutputDebugString(msg);
                        }
                        CloseHandle(hProcess);
                    }
                    break;  
                }
            }

        } while (Process32Next(processesSnapshot, &processInfo));
    }

    CloseHandle(processesSnapshot);
}

//제일 마지막에 생성된 브라우저 PID
DWORD CcheckBrowserDlg::FindBrowserPIDsLast()
{
    CString browsers[] = { _T("chrome.exe"), _T("msedge.exe"), _T("iexplore.exe") };

    PROCESSENTRY32 processInfo = {0};
    processInfo.dwSize = sizeof(PROCESSENTRY32);

    HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (processesSnapshot == INVALID_HANDLE_VALUE)
        return 0;

    DWORD latestPID = 0;
    FILETIME latestCreationTime = {0, 0}; // 초기값은 0

    if (Process32First(processesSnapshot, &processInfo))
    {
        do
        {
            bool isBrowser = false;
            for (int i = 0; i < _countof(browsers); ++i)
            {
                if (browsers[i].CompareNoCase(processInfo.szExeFile) == 0)
                {
                    isBrowser = true;
                    break;
                }
            }
            if (!isBrowser)
                continue;

            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, processInfo.th32ProcessID);
            if (hProcess != NULL)
            {
                FILETIME creationTime, exitTime, kernelTime, userTime;
                if (GetProcessTimes(hProcess, &creationTime, &exitTime, &kernelTime, &userTime))
                {
                    // 가장 최근 생성 시간과 비교
                    if (CompareFileTime(&creationTime, &latestCreationTime) == 1)
                    {
                        latestCreationTime = creationTime;
                        latestPID = processInfo.th32ProcessID;
                    }
                }
                CloseHandle(hProcess);
            }
        } while (Process32Next(processesSnapshot, &processInfo));
    }

    CloseHandle(processesSnapshot);
    return latestPID; // 찾은 가장 최근 프로세스 PID 반환, 없으면 0
}

void CcheckBrowserDlg::OnBnClickedButton1()
{
	DWORD pid = CFunc::ExecuteChromeBrowser(_T("https://www.naver.com"));    
}

void CcheckBrowserDlg::OnBnClickedButton2()
{
	//iexplore.exe 실행 → IE 자체는 더 이상 사용되지 않고 → msedge.exe (Edge)로 전환됨
    //pid = FindLastCreatedProcess(_T("iexplore.exe"));
	DWORD pid = CFunc::ExecuteIEBrowser(_T("https://www.naver.com"));    
}

void CcheckBrowserDlg::OnBnClickedButton3()
{
	DWORD pid = CFunc::ExecuteEdgeBrowser(_T("https://www.naver.com"));
}
