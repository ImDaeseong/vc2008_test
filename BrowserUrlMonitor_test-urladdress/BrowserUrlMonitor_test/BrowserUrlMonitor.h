#pragma once

#include <afxtempl.h>
#include <afxmt.h>

class CBrowserUrlMonitor
{
public:
    static CBrowserUrlMonitor* GetInstance();
    static void ReleaseInstance();

    BOOL IsSupportedBrowser(HWND hwnd);
    BOOL GetLastKnownUrl(HWND hwnd, CString& szRetUrl);

private:
    CBrowserUrlMonitor(void);
    ~CBrowserUrlMonitor(void);

    BOOL StartHook();
    void StopHook();
    
    BOOL ValidateDomain(const CString& szUrl);
    BOOL IsMatchingProcess(const CString& szProcessName);
    void UpdateUrlMap(HWND hwnd, const CString& szUrl);

    static void CALLBACK WinEventProc(HWINEVENTHOOK hook, DWORD dwEvent, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime);
    static void ExtractAndStoreUrl(HWND hwnd, LONG idObject, LONG idChild);

private:
    static CBrowserUrlMonitor* m_pInstance;
    static CCriticalSection m_InstanceLock;

    HWINEVENTHOOK m_hEventHook;
    CMap<HWND, HWND, CString, CString> m_mapBrowserUrl;
	CCriticalSection m_DataLock;
};