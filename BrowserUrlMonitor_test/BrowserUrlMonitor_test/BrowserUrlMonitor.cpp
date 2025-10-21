#include "stdafx.h"
#include "BrowserUrlMonitor.h"
#include "BrowserUrlMonitor_testDlg.h"

CBrowserUrlMonitor::CBrowserUrlMonitor()
{
	m_hEventHook = NULL;
}

CBrowserUrlMonitor::~CBrowserUrlMonitor()
{
    StopMonitor();
}

BOOL CBrowserUrlMonitor::StartMonitor()
{
    if (m_hEventHook == NULL)
    {
        m_hEventHook = SetWinEventHook(EVENT_OBJECT_VALUECHANGE, EVENT_OBJECT_VALUECHANGE, NULL, HandleWinEvent, 0, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);
    	if(m_hEventHook == NULL)
			return FALSE;
	}

    return TRUE;
}

void CBrowserUrlMonitor::StopMonitor()
{
    if (m_hEventHook)
    {
        UnhookWinEvent(m_hEventHook);
        m_hEventHook = NULL;
    }
}

void CALLBACK CBrowserUrlMonitor::HandleWinEvent(HWINEVENTHOOK, DWORD, HWND hwnd, LONG idObject, LONG idChild, DWORD, DWORD)
{
    if (hwnd && IsBrowserClass(hwnd))
    {
		CBrowserUrlMonitor::ProcessEvent(hwnd, idObject, idChild);
    }
}

BOOL CBrowserUrlMonitor::IsBrowserClass(HWND hwnd)
{
    char szClass[MAX_PATH] = {0};
    GetClassNameA(hwnd, szClass, MAX_PATH);

    if (_stricmp(szClass, "Chrome_WidgetWin_1") == 0 ||_stricmp(szClass, "IEFrame") == 0 || _stricmp(szClass, "Windows.UI.Core.CoreWindow") == 0)
    {
        return TRUE;
    }
    return FALSE;
}

void CBrowserUrlMonitor::ProcessEvent(HWND hwnd, LONG idObject, LONG idChild)
{
    IAccessible* pAcc = NULL;
    VARIANT varChild;
    VariantInit(&varChild);

    HRESULT hr = AccessibleObjectFromEvent(hwnd, idObject, idChild, &pAcc, &varChild);
    if (SUCCEEDED(hr) && pAcc != NULL)
    {
        BSTR bstrValue = NULL;
        if (SUCCEEDED(pAcc->get_accValue(varChild, &bstrValue)) && bstrValue != NULL)
        {
            CString strUrl(bstrValue);
			SysFreeString(bstrValue);

            if (!strUrl.IsEmpty())
            {
				BrowserUrlInfo* pInfo = new BrowserUrlInfo();
                pInfo->hwndBrowser = hwnd;
                pInfo->strUrl = strUrl;

				CWinApp* pApp = AfxGetApp();
				if(pApp != NULL && pApp->m_pMainWnd != NULL)
				{									
					if(!pApp->m_pMainWnd->PostMessage(WM_URL_DETECTED, 0, (LPARAM)pInfo))
						delete pInfo;
				}
            }            
        }
        pAcc->Release();
    }

    VariantClear(&varChild);
}