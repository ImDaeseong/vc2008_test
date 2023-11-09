#include "stdafx.h"
#include "myservice.h"


SERVICE_STATUS        ServiceStatus;
SERVICE_STATUS_HANDLE hStatus;
TCHAR szServiceName[] = _T("myservice");


// �α� ���� ��� ����
TCHAR szLogFilePath[MAX_PATH];

// ���� �ڵ� ����
HANDLE g_hLogFile = NULL;


void ServiceMain(int argc, _TCHAR* argv[]);
void WINAPI ServiceCtrlHandler(DWORD request);
DWORD WINAPI ServiceWorkerThread(LPVOID lpParam);


// ������ ���� �ɼ�
BOOL g_bServiceStop = FALSE;



int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // �α� ���� ���� ���
    _tcscpy_s(szLogFilePath, _T("C:\\log.txt"));

    SERVICE_TABLE_ENTRY ServiceTable[] = {
        { szServiceName, (LPSERVICE_MAIN_FUNCTION)ServiceMain },
        { NULL, NULL }
    };

    if (StartServiceCtrlDispatcher(ServiceTable) == FALSE) 
	{
        return GetLastError();
    }

    return 0;
}


void ServiceMain(int argc, _TCHAR* argv[]) {

    hStatus = RegisterServiceCtrlHandler(szServiceName, ServiceCtrlHandler);
    if (hStatus == (SERVICE_STATUS_HANDLE)0)
        return;

    ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
    ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    ServiceStatus.dwWin32ExitCode = 0;
    ServiceStatus.dwServiceSpecificExitCode = 0;
    ServiceStatus.dwCheckPoint = 0;
    ServiceStatus.dwWaitHint = 0;

    SetServiceStatus(hStatus, &ServiceStatus);

    // ��׶��� ������ ����
    HANDLE hThread = CreateThread(NULL, 0, ServiceWorkerThread, NULL, 0, NULL);
    if (hThread == NULL)
	{
        ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        ServiceStatus.dwWin32ExitCode = GetLastError();
        SetServiceStatus(hStatus, &ServiceStatus);
        return;
    }

    // ���� ���� ������ ����
    ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(hStatus, &ServiceStatus);

    // �α� ���� ���� (ANSI ���ڵ�)
    g_hLogFile = CreateFile(szLogFilePath, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    // �����尡 ����� ������ ���
    WaitForSingleObject(hThread, INFINITE);

    // �α� ���� �ݱ�
    if (g_hLogFile != NULL)
        CloseHandle(g_hLogFile);

    // ���񽺰� ���� ����
    ServiceStatus.dwCurrentState = SERVICE_STOPPED;
    SetServiceStatus(hStatus, &ServiceStatus);
}

DWORD WINAPI ServiceWorkerThread(LPVOID lpParam) {

    while (!g_bServiceStop)
	{

        SYSTEMTIME st;
        GetLocalTime(&st);

        // �ѱ� ���ڿ��� ANSI ���ڿ��� ����
        char tMessage[1024];
        int result = sprintf_s(tMessage, _countof(tMessage), "����ð�: %02d:%02d:%02d\n", st.wHour, st.wMinute, st.wSecond);
        if (result > 0) 
		{
            if (g_hLogFile != NULL) 
			{
                DWORD bytesWritten;
                WriteFile(g_hLogFile, tMessage, result, &bytesWritten, NULL);
            }
        }

        // 5�ʸ��� �ݺ�
        Sleep(5000);
    }

    return 0;
}

void WINAPI ServiceCtrlHandler(DWORD request) {

    switch (request) {
    case SERVICE_CONTROL_STOP:
    case SERVICE_CONTROL_SHUTDOWN:
        ServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
        ServiceStatus.dwControlsAccepted = 0;
        SetServiceStatus(hStatus, &ServiceStatus);

        // ���� �ɼ�
        g_bServiceStop = TRUE;
        break;

    default:
        break;
    }
}
