#pragma once

struct ProcessInfo {
    DWORD pid;
    std::string fullPath;
};

class CheckSuspend
{
public:
	CheckSuspend();
	~CheckSuspend();
	 
	bool PauseResumeThreadList(DWORD dwOwnerPID, bool bResumeThread);
    std::vector<ProcessInfo> GetPIDs(const char* szFileName);
    bool IsProcessSuspended(DWORD processID);
	bool IsProcessRunning(DWORD processID);

private:
	std::vector<DWORD> GetProcessThreads(DWORD processID);
    void ToLower(char* str);
    void GetProcessFullPath(DWORD processID, std::string& fullPath);
};
