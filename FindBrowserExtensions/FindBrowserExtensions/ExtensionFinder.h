#pragma once

#include "ExtensionsInfo.h"

class CExtensionFinder
{
public:
	CExtensionFinder(HWND hwnd);
	~CExtensionFinder(void);

private:
	HWND m_hWnd;
	HANDLE m_hKillEvent;
	HANDLE m_hThread;
	DWORD m_dwThreadId;
	bool m_bCheckThread;

	static const DWORD TIMEOUT_IN_MS = 1000;

	UINT ExtensionFinderParam();
	static DWORD WINAPI ExtensionFinderThread(LPVOID pParam);

	void initSet();

public:
	BOOL StartExtensionFinder();
	BOOL StopExtensionFinder();
	std::vector<AddonInfo> getAddonInfo() const;

private:
	ExtensionsInfo m_info;	
};
