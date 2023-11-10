#include "StdAfx.h"
#include "ExtensionFinder.h"

CExtensionFinder::CExtensionFinder(HWND hwnd) : m_hKillEvent(NULL), m_hThread(NULL), m_dwThreadId(0), m_hWnd(hwnd), m_bCheckThread(false)
{
}

CExtensionFinder::~CExtensionFinder(void)
{
	// 리소스 해제 전 스레드 중지
    StopExtensionFinder(); 

    if (m_hKillEvent)
        CloseHandle(m_hKillEvent);

    if (m_hThread)
        CloseHandle(m_hThread);
}

void CExtensionFinder::initSet()
{
	m_info.InitPath();
}

BOOL CExtensionFinder::StartExtensionFinder()
{
    if (m_bCheckThread)
    {
        return TRUE;
    }

    // 스레드 초기화 데이터 설정
	initSet();


    m_bCheckThread = true;
    m_hKillEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    m_hThread = CreateThread(NULL, 0, ExtensionFinderThread, this, 0, &m_dwThreadId);

    if (m_hThread == NULL)
    {
        m_bCheckThread = false;

        // 리소스 누수를 방지하기 위해 이벤트 핸들 닫기
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
		// 이벤트를 설정하여 스레드에게 중지 요청 
        ::SetEvent(m_hKillEvent);

        // 정상적인 종료를 위해 타임아웃과 함께 대기
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
		// m_hKillEvent 이벤트가 시그널(발생)되면 스레드 중지
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
	//찾을 경로 정보
	//m_info.PathExtensionInfo();

	//정보를 검색
	m_info.FindExtension();

	//정보 검색이 완료 되면 스레드 중지
	StopExtensionFinder();

	/*
	std::vector<AddonInfo> addons = m_info.getAddonInfo();
	for (int i = 0; i < addons.size(); i++)
	{
		CString strMsg;
		strMsg.Format(_T("%s %s %s \n"), addons[i].strItem, addons[i].strDisplayName, addons[i].strItemFullPath);
		OutputDebugString(strMsg);
	}
	*/

	//완료 여부를 메인으로 전달 
	PostMessage(m_hWnd, WM_FINDEXTENSION_DONE, NULL, NULL);

    return 0;
}

std::vector<AddonInfo> CExtensionFinder::getAddonInfo() const
{
    return m_info.getAddonInfo();
}