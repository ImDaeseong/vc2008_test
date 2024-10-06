#include "StdAfx.h"
#include "CheckSuspend.h"

CheckSuspend::CheckSuspend()
{
}

CheckSuspend::~CheckSuspend()
{
}

//���μ��� �Ͻ�����/����
bool CheckSuspend::PauseResumeThreadList(DWORD dwOwnerPID, bool bResumeThread)
{
    std::vector<DWORD> threads = GetProcessThreads(dwOwnerPID);
    
    for (size_t i = 0; i < threads.size(); ++i)
    {
        HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, threads[i]);
        if (hThread != NULL)
        {
            if (bResumeThread)
                ResumeThread(hThread);
            else
                SuspendThread(hThread);
            
            CloseHandle(hThread);
        }
    }
    return !threads.empty();
}

//���μ��� ID
std::vector<ProcessInfo> CheckSuspend::GetPIDs(const char* szFileName)
{
    std::vector<ProcessInfo> processes;
    char szFileLower[MAX_PATH];
    strncpy(szFileLower, szFileName, MAX_PATH);
    szFileLower[MAX_PATH - 1] = '\0';
    ToLower(szFileLower);
    
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE)
        return processes;
    
    PROCESSENTRY32 proc = { sizeof(PROCESSENTRY32) };
    if (Process32First(snapshot, &proc))
    {
        do
        {
            char exeFile[MAX_PATH];
            strncpy(exeFile, proc.szExeFile, MAX_PATH);
            exeFile[MAX_PATH - 1] = '\0';
            ToLower(exeFile);
            if (strcmp(szFileLower, exeFile) == 0)
            {
                ProcessInfo info;
                info.pid = proc.th32ProcessID;
                GetProcessFullPath(info.pid, info.fullPath);
                processes.push_back(info);
            }
        } while (Process32Next(snapshot, &proc));
    }
    CloseHandle(snapshot);
    return processes;
}

//���μ����� �Ͻ� ���� ����
bool CheckSuspend::IsProcessSuspended(DWORD processID)
{
    std::vector<DWORD> threads = GetProcessThreads(processID);
    
    for (size_t i = 0; i < threads.size(); ++i)
    {
        HANDLE hThread = OpenThread(THREAD_QUERY_INFORMATION, FALSE, threads[i]);
        if (hThread != NULL)
        {
			//�Ͻ����� ���̸� 
            DWORD suspendCount = SuspendThread(hThread);
            if (suspendCount != (DWORD)-1)
            {
				//�����
                ResumeThread(hThread);
                CloseHandle(hThread);
                return suspendCount > 0;
            }
            CloseHandle(hThread);
        }
    }
    return false;
}

//���μ��� ID �߰�
std::vector<DWORD> CheckSuspend::GetProcessThreads(DWORD processID)
{
    std::vector<DWORD> threads;
    HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    
    if (hThreadSnap != INVALID_HANDLE_VALUE)
    {
        THREADENTRY32 te32 = { sizeof(THREADENTRY32) };
        if (Thread32First(hThreadSnap, &te32))
        {
            do
            {
                if (te32.th32OwnerProcessID == processID)
                {
                    threads.push_back(te32.th32ThreadID);
                }
            } while (Thread32Next(hThreadSnap, &te32));
        }
        CloseHandle(hThreadSnap);
    }
    return threads;
}

//���μ��� �������� ���
void CheckSuspend::GetProcessFullPath(DWORD processID, std::string& fullPath)
{
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
    if (hProcess != NULL)
    {
        char path[MAX_PATH];
        if (GetModuleFileNameEx(hProcess, NULL, path, MAX_PATH) != 0)
        {
            fullPath = path;
        }
        CloseHandle(hProcess);
    }
}

//���μ��� ���� ���� üũ
bool CheckSuspend::IsProcessRunning(DWORD processID)
{
    HANDLE hProcess = OpenProcess(SYNCHRONIZE, FALSE, processID);
    if (hProcess != NULL)
    {
        CloseHandle(hProcess);
        return true;
    }
    return false;
}

void CheckSuspend::ToLower(char* str)
{
    for (int i = 0; str[i]; ++i)
    {
        str[i] = tolower(str[i]);
    }
}