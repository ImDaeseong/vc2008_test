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
	m_info.InitPath();
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
		// �̺�Ʈ�� �����Ͽ� �����忡�� ���� ��û 
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
		// m_hKillEvent �̺�Ʈ�� �ñ׳�(�߻�)�Ǹ� ������ ����
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
	//ã�� ��� ����
	//m_info.PathExtensionInfo();

	//������ �˻�
	m_info.FindExtension();

	//���� �˻��� �Ϸ� �Ǹ� ������ ����
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

	//�Ϸ� ���θ� �������� ���� 
	PostMessage(m_hWnd, WM_FINDEXTENSION_DONE, NULL, NULL);

    return 0;
}

std::vector<AddonInfo> CExtensionFinder::getAddonInfo() const
{
    return m_info.getAddonInfo();
}