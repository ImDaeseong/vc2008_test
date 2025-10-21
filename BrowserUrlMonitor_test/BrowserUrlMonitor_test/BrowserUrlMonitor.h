#pragma once
#include <windows.h>
#include <oleacc.h>
#include <atlstr.h>
#pragma comment(lib, "Oleacc.lib")

struct BrowserUrlInfo
{
    HWND hwndBrowser;
    CString strUrl;
};

class CBrowserUrlMonitor
{
public:
    CBrowserUrlMonitor();
    ~CBrowserUrlMonitor();
    
    BOOL StartMonitor();
    void StopMonitor();

private:
    static void CALLBACK HandleWinEvent(HWINEVENTHOOK hook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD idThread, DWORD dwmsEventTime);
    static void ProcessEvent(HWND hwnd, LONG idObject, LONG idChild);
    static BOOL IsBrowserClass(HWND hwnd);

private:
    HWINEVENTHOOK m_hEventHook;
};
