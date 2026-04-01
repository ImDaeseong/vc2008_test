#include "StdAfx.h"
#include "BrowserUrlMonitor.h"
#include <Psapi.h>
#include <oleacc.h>

#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "oleacc.lib")

CBrowserUrlMonitor* CBrowserUrlMonitor::m_pInstance = NULL;
CCriticalSection CBrowserUrlMonitor::m_InstanceLock;

CBrowserUrlMonitor* CBrowserUrlMonitor::GetInstance()
{
    CSingleLock lock(&m_InstanceLock, TRUE);
    if (m_pInstance == NULL)
    {
        m_pInstance = new CBrowserUrlMonitor();
        if (!m_pInstance->StartHook())
        {
            delete m_pInstance;
            m_pInstance = NULL;
        }
    }
    return m_pInstance;
}

void CBrowserUrlMonitor::ReleaseInstance()
{
    CSingleLock lock(&m_InstanceLock, TRUE);
    if (m_pInstance != NULL)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

CBrowserUrlMonitor::CBrowserUrlMonitor(void) : m_hEventHook(NULL)
{
}

CBrowserUrlMonitor::~CBrowserUrlMonitor(void)
{
    StopHook();
}

BOOL CBrowserUrlMonitor::StartHook()
{
    if (m_hEventHook == NULL)
    {
        //EVENT_OBJECT_VALUECHANGE를 통해 주소창 편집 감지
        m_hEventHook = SetWinEventHook(
            EVENT_OBJECT_VALUECHANGE, EVENT_OBJECT_VALUECHANGE,
            NULL,
            CBrowserUrlMonitor::WinEventProc,
            0, 0,
            WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);
    }
    return (m_hEventHook != NULL);
}

void CBrowserUrlMonitor::StopHook()
{
    if (m_hEventHook != NULL)
    {
        UnhookWinEvent(m_hEventHook);
        m_hEventHook = NULL;
    }

    CSingleLock lock(&m_DataLock, TRUE);
    m_mapBrowserUrl.RemoveAll();
}

BOOL CBrowserUrlMonitor::IsSupportedBrowser(HWND hwnd)
{
    if (!::IsWindow(hwnd)) return FALSE;

    char szClassName[MAX_PATH] = {0};
    GetClassNameA(hwnd, szClassName, MAX_PATH);

    //Chromium 기반 브라우저들의 공통 윈도우 클래스명
    if (_stricmp(szClassName, "Chrome_WidgetWin_1") == 0)
    {
        DWORD dwPid = 0;
        GetWindowThreadProcessId(hwnd, &dwPid);
        
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, dwPid);
        if (hProcess)
        {
            char szPath[MAX_PATH] = {0};
            if (GetProcessImageFileNameA(hProcess, szPath, MAX_PATH))
            {
                CString strPath(szPath);
                int nPos = strPath.ReverseFind('\\');
                if (nPos >= 0)
                {
                    if (IsMatchingProcess(strPath.Mid(nPos + 1)))
                    {
                        CloseHandle(hProcess);
                        return TRUE;
                    }
                }
            }
            CloseHandle(hProcess);
        }
    }
    return FALSE;
}

BOOL CBrowserUrlMonitor::IsMatchingProcess(const CString& szProcessName)
{
    //대상 브라우저 목록
    const char* targets[] = { "chrome.exe", "msedge.exe", "whale.exe", "brave.exe", NULL };
    for (int i = 0; targets[i] != NULL; i++)
    {
        if (szProcessName.CompareNoCase(targets[i]) == 0) return TRUE;
    }
    return FALSE;
}

BOOL CBrowserUrlMonitor::ValidateDomain(const CString& szUrl)
{
    if (szUrl.IsEmpty()) return FALSE;

    //프로토콜 제거 후 도메인 추출 시도
    CString strClean = szUrl;
    strClean.Replace("https://", "");
    strClean.Replace("http://", "");

    int nSlash = strClean.Find('/');
    if (nSlash > 0) strClean = strClean.Left(nSlash);

    //최소한 '.' 하나는 포함되어야 함 (예: google.com)
    return (strClean.Find('.') > 0);
}

void CBrowserUrlMonitor::UpdateUrlMap(HWND hwnd, const CString& szUrl)
{
    CSingleLock lock(&m_DataLock, TRUE);
    m_mapBrowserUrl.SetAt(hwnd, szUrl);
}

BOOL CBrowserUrlMonitor::GetLastKnownUrl(HWND hwnd, CString& szRetUrl)
{
    CSingleLock lock(&m_DataLock, TRUE);
    return m_mapBrowserUrl.Lookup(hwnd, szRetUrl);
}

void CALLBACK CBrowserUrlMonitor::WinEventProc(HWINEVENTHOOK hook, DWORD dwEvent, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime)
{
    if (hwnd == NULL || dwEvent != EVENT_OBJECT_VALUECHANGE) return;

    CBrowserUrlMonitor* pMonitor = GetInstance();
    if (pMonitor && pMonitor->IsSupportedBrowser(hwnd))
    {
        ExtractAndStoreUrl(hwnd, idObject, idChild);
    }
}

void CBrowserUrlMonitor::ExtractAndStoreUrl(HWND hwnd, LONG idObject, LONG idChild)
{
    IAccessible* pAcc = NULL;
    VARIANT varChild;
    VariantInit(&varChild);

    if (AccessibleObjectFromEvent(hwnd, idObject, idChild, &pAcc, &varChild) == S_OK && pAcc)
    {
        BSTR bstrValue = NULL;
        if (pAcc->get_accValue(varChild, &bstrValue) == S_OK && bstrValue != NULL)
        {
            CString szUrl(bstrValue);
            CBrowserUrlMonitor* pMonitor = GetInstance();
            
            if (!szUrl.IsEmpty() && pMonitor->ValidateDomain(szUrl))
            {
                pMonitor->UpdateUrlMap(hwnd, szUrl);
            }
            SysFreeString(bstrValue);
        }
        pAcc->Release();
    }
    VariantClear(&varChild);
}