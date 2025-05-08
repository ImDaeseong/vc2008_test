#pragma once

struct LoLDataInfo
{
	CString strGameName;
	CString strGameTag;
	CString strGameId;
};

struct LockFileInfo
{
    CString strProcessName;  
    int nPid;              
    int nPort;             
    CString strPassword;     
    CString strProtocol;    
};

class CLoLManager
{
public:
    CLoLManager();
    ~CLoLManager();

	LoLDataInfo GetGameDataInfo();

private:
	CString GetLockFilePath();
	LockFileInfo ParseLockFile(const CString& strPath);
	CString MakeAuthHeader(const CString& strPassword);

private:
    std::string Base64Encode(const std::string& strInput);
    char* Utf8ToChar(const char* chInput, size_t length);
    CString GetJson(const CString& strUrl, const CString& strHeaders, unsigned short uPort);
};