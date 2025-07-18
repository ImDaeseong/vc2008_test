#pragma once

class CFunc
{
public:
	CFunc();
	~CFunc();

public:
	static CString GetProgramFilesFolder();
	static CString GetProgramFilesX86Folder();
	static CString GetChromePath();
	static CString GetIePath();
	static CString GetEdgePath();
	static DWORD ExecuteChromeBrowser(CString strUrl);
	static DWORD ExecuteIEBrowser(CString strUrl);
	static DWORD ExecuteEdgeBrowser(CString strUrl);
};
