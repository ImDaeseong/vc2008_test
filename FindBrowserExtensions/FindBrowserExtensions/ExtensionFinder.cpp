#include "StdAfx.h"
#include "ExtensionFinder.h"

CExtensionFinder::CExtensionFinder(HWND hwnd) : m_hKillEvent(NULL), m_hThread(NULL), m_dwThreadId(0), m_hWnd(hwnd), m_bCheckThread(false)
{
}

CExtensionFinder::~CExtensionFinder(void)
{
	// ���ҽ� ���� �� ������ ����
    StopExtensionFinder(); 

    if (m_hKillEvent)
        CloseHandle(m_hKillEvent);

    if (m_hThread)
        CloseHandle(m_hThread);
}

void CExtensionFinder::initSet()
{

}

BOOL CExtensionFinder::StartExtensionFinder()
{
    if (m_bCheckThread)
    {
        return TRUE;
    }

    // ������ �ʱ�ȭ ������ ����
	initSet();


    m_bCheckThread = true;
    m_hKillEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    m_hThread = CreateThread(NULL, 0, ExtensionFinderThread, this, 0, &m_dwThreadId);

    if (m_hThread == NULL)
    {
        m_bCheckThread = false;

        // ���ҽ� ������ �����ϱ� ���� �̺�Ʈ �ڵ� �ݱ�
        if (m_hKillEvent)
            CloseHandle(m_hKillEvent);

        return FALSE;
    }

    return TRUE;
}
	
BOOL CExtensionFinder::StopExtensionFinder()
{
    m_bCheckThread = false;
    DWORD dwExitCode;

    if (m_hThread != NULL && ::GetExitCodeThread(m_hThread, &dwExitCode) && dwExitCode == STILL_ACTIVE)
    {
        ::SetEvent(m_hKillEvent);

        // �������� ���Ḧ ���� Ÿ�Ӿƿ��� �Բ� ���
        ::WaitForSingleObject(m_hThread, TIMEOUT_IN_MS);

        m_hThread = NULL;
        m_hKillEvent = NULL;

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

DWORD CExtensionFinder::ExtensionFinderThread(LPVOID pParam)
{
    CExtensionFinder* pThis = static_cast<CExtensionFinder*>(pParam);

    while (pThis->m_bCheckThread)
    {
        if (::WaitForSingleObject(pThis->m_hKillEvent, TIMEOUT_IN_MS) == WAIT_OBJECT_0)
        {
            pThis->m_bCheckThread = false;
            break;
        }
        else
        {
            pThis->ExtensionFinderParam();
        }
    }

    return 0; 
}

UINT CExtensionFinder::ExtensionFinderParam()
{
    SYSTEMTIME st;
    GetLocalTime(&st);
    TRACE(_T("ExtensionFinderParam: %02d:%02d:%02d\n"), st.wHour, st.wMinute, st.wSecond);


    return 0;
}