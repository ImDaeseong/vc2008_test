#pragma once

class RegInfo
{
public:
	RegInfo();
	~RegInfo();

public:
	DWORD   GetDotNetFrameworkVersion(LPCTSTR pszSubKey, LPCTSTR pszValueName);
	bool    GetRedistPackageInstalled(LPCTSTR pszBaseKey, LPCTSTR pszSubKey);
	CString GetEdgeVersion(LPCTSTR pszSubKey, LPCTSTR pszValueName);
	bool    GetMariaDBPackageInstalled(LPCTSTR pszBaseKey, LPCTSTR pszSubKey);
	CString GetMariaDBPath(LPCTSTR pszValueName);
};
