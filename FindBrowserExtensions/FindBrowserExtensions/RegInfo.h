#pragma once

class RegInfo
{
public:
	RegInfo(void);
	~RegInfo(void);

	CString GetAppDataPath();
	CString GetLocalAppDataPath();
};
