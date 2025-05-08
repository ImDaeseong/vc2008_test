#include "../stdafx.h"
#include "LoLManager.h"

CLoLManager::CLoLManager()
{
}

CLoLManager::~CLoLManager() 
{
}

std::string CLoLManager::Base64Encode(const std::string& strInput)
{
    static const char* base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string output;
    int val = 0, valb = -6;

    for (size_t i = 0; i < strInput.size(); ++i)
    {
        unsigned char c = strInput[i];
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0)
        {
            output.push_back(base64_chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }

    if (valb > -6)
        output.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);

    while (output.size() % 4)
        output.push_back('=');

    return output;
}

char* CLoLManager::Utf8ToChar(const char* chInput, size_t length)
{
    int nWideCharSize = MultiByteToWideChar(CP_UTF8, 0, chInput, static_cast<int>(length), NULL, 0);
    if (nWideCharSize == 0)
        return NULL;

    WCHAR* wideCharStr = new WCHAR[nWideCharSize + 1];
    ZeroMemory(wideCharStr, sizeof(WCHAR) * (nWideCharSize + 1));
    MultiByteToWideChar(CP_UTF8, 0, chInput, static_cast<int>(length), wideCharStr, nWideCharSize);

    int nCharSize = WideCharToMultiByte(CP_ACP, 0, wideCharStr, nWideCharSize, NULL, 0, NULL, NULL);
    if (nCharSize == 0)
    {
        delete[] wideCharStr;
        return NULL;
    }

    char* charStr = new char[nCharSize + 1];
    ZeroMemory(charStr, nCharSize + 1);
    WideCharToMultiByte(CP_ACP, 0, wideCharStr, nWideCharSize, charStr, nCharSize, NULL, NULL);

    delete[] wideCharStr;
    return charStr;
}

CString CLoLManager::GetJson(const CString& strUrl, const CString& strHeaders, unsigned short uPort)
{
    CString strServerDomainNoHttp;
    CString strServerDomain;
    CString strPageName;

    int nHttpIndex = -1;
    int nPlusIndex = 7;

    int nFind = strUrl.Find('/', 10);
    if (nFind < 0)
        return _T("");

    strServerDomain = strUrl.Left(nFind);
    strPageName = strUrl.Mid(nFind + 1);

    nHttpIndex = strServerDomain.Find(_T("http://"));
    if (nHttpIndex < 0)
    {
        nHttpIndex = strServerDomain.Find(_T("https://"));
        nPlusIndex++;
    }

    if (nHttpIndex != -1)
        strServerDomainNoHttp = strServerDomain.Mid(nHttpIndex + nPlusIndex);
    else
        strServerDomainNoHttp = strServerDomain;

    HINTERNET hInternet = InternetOpen(_T("HTTP"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (hInternet == NULL)
        return _T("");

    DWORD dwTimeout = 5000; // 5 seconds
    InternetSetOption(hInternet, INTERNET_OPTION_CONNECT_TIMEOUT, &dwTimeout, sizeof(dwTimeout));
    InternetSetOption(hInternet, INTERNET_OPTION_RECEIVE_TIMEOUT, &dwTimeout, sizeof(dwTimeout));
    InternetSetOption(hInternet, INTERNET_OPTION_SEND_TIMEOUT, &dwTimeout, sizeof(dwTimeout));

    HINTERNET hConnect = InternetConnect(hInternet, strServerDomainNoHttp, uPort, _T(""), _T(""), INTERNET_SERVICE_HTTP, 0, 0);
    if (hConnect == NULL)
    {
        InternetCloseHandle(hInternet);
        return _T("");
    }

    HINTERNET hRequest = HttpOpenRequest(hConnect, _T("GET"), strPageName, HTTP_VERSION, _T(""), NULL, INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID, 0);
    if (hRequest == NULL)
    {
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return _T("");
    }

    DWORD dwFlags = SECURITY_FLAG_IGNORE_UNKNOWN_CA | SECURITY_FLAG_IGNORE_REVOCATION | SECURITY_FLAG_IGNORE_REDIRECT_TO_HTTP | SECURITY_FLAG_IGNORE_REDIRECT_TO_HTTPS | SECURITY_FLAG_IGNORE_CERT_DATE_INVALID | SECURITY_FLAG_IGNORE_CERT_CN_INVALID;
    InternetSetOption(hRequest, INTERNET_OPTION_SECURITY_FLAGS, &dwFlags, sizeof(dwFlags));
    HttpAddRequestHeaders(hRequest, strHeaders, strHeaders.GetLength(), HTTP_ADDREQ_FLAG_REPLACE | HTTP_ADDREQ_FLAG_ADD);

    BOOL bSend = HttpSendRequest(hRequest, NULL, 0, NULL, 0);
    if (!bSend)
    {
        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return _T("");
    }

    CString strReturn = _T("");
    char szBuff[10000] = { 0 };
    DWORD dwReadSize = 0;

    while (InternetReadFile(hRequest, szBuff, sizeof(szBuff) - 1, &dwReadSize) && dwReadSize > 0)
    {
        szBuff[dwReadSize] = '\0';
        char* pConverted = Utf8ToChar(szBuff, dwReadSize);
        if (pConverted)
        {
            strReturn += pConverted;
            delete[] pConverted;
        }
    }

    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    return strReturn;
}

CString CLoLManager::GetLockFilePath()
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
        return _T("");

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnapshot, &pe))
    {
        do
        {
            //OutputDebugString(pe.szExeFile);

            if (_tcsicmp(pe.szExeFile, _T("LeagueClientUx.exe")) == 0)
            {
                HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe.th32ProcessID);
                if (hProcess != NULL)
                {
                    TCHAR tPath[MAX_PATH] = { 0 };
                    if (GetModuleFileNameEx(hProcess, NULL, tPath, MAX_PATH))
                    {
                        PathRemoveFileSpec(tPath);

						CString strPath(tPath);
                        if (strPath.Right(1) != _T("\\")) 
                            strPath += _T("\\");
                        strPath += _T("lockfile");

                        CloseHandle(hProcess);
                        CloseHandle(hSnapshot);
                        return CString(strPath);
                    }
                    CloseHandle(hProcess);
                }
            }
        } while (Process32Next(hSnapshot, &pe));
    }
    CloseHandle(hSnapshot);

    return _T("");
}

LockFileInfo CLoLManager::ParseLockFile(const CString& strPath)
{
    LockFileInfo lockFileInfo;

    CFile file;
    if (!file.Open(strPath, CFile::modeRead | CFile::shareDenyNone))
        return LockFileInfo(); 

    ULONGLONG length = file.GetLength();

    char* buffer = new char[(size_t)length + 1];
    memset(buffer, 0, (size_t)length + 1);

    file.Read(buffer, (UINT)length);
    buffer[length] = '\0';

    CString data(buffer);
    delete[] buffer;  

    int pos = 0;
	lockFileInfo.strProcessName = data.Tokenize(_T(":"), pos);
	lockFileInfo.nPid = _ttoi(data.Tokenize(_T(":"), pos));
	lockFileInfo.nPort = _ttoi(data.Tokenize(_T(":"), pos));
	lockFileInfo.strPassword = data.Tokenize(_T(":"), pos);
	lockFileInfo.strProtocol = data.Tokenize(_T(":"), pos);

    file.Close();

    return lockFileInfo;
}

CString CLoLManager::MakeAuthHeader(const CString& strPassword)
{
	std::string strKey = _T("riot:") + strPassword;

	CString strResult;
	strResult.Format(_T("Authorization: Basic %s\r\n"), Base64Encode(strKey).c_str());

	return strResult;
}

LoLGameInfo CLoLManager::GetDataInfo()
{
    LoLGameInfo item;

    CString strPath = GetLockFilePath();
    if (strPath.IsEmpty())
    {
        return item;
    }

    LockFileInfo lockFileInfo = ParseLockFile(strPath);
    if (lockFileInfo.strProtocol.IsEmpty() || lockFileInfo.strPassword.IsEmpty())
    {
        return item;
    }

	CString strHeaders = MakeAuthHeader(lockFileInfo.strPassword);
  
	CString strUrl;
	strUrl.Format(_T("%s://127.0.0.1/lol-chat/v1/me"), lockFileInfo.strProtocol.GetString());
	
    std::string response = GetJson(strUrl, strHeaders, lockFileInfo.nPort);
    //OutputDebugString(response.c_str());

	Json::Reader reader;
	Json::Value root;
	bool parsingRet = reader.parse(response, root);
	if (!parsingRet)
    {
        return item;
    }

	if (root.isObject())
	{
		try
		{
			CString availability = root["availability"].asString().c_str();
			CString gameName = root["gameName"].asString().c_str();
			CString gameTag = root["gameTag"].asString().c_str();
			CString id = root["id"].asString().c_str();
			CString name = root["name"].asString().c_str();
			CString patchline = root["patchline"].asString().c_str();
			CString pid = root["pid"].asString().c_str();
			CString platformId = root["platformId"].asString().c_str();
			CString product = root["product"].asString().c_str();
			CString productName = root["productName"].asString().c_str();
			CString puuid = root["puuid"].asString().c_str();
			CString statusMessage = root["statusMessage"].asString().c_str();
			CString summary = root["summary"].asString().c_str();
			CString summonerId = root["summonerId"].asString().c_str();

			item.strGameName = gameName;
			item.strGameTag = gameTag;
		
			Json::Value lolInfo = root["lol"];
			if (lolInfo.isObject())
			{
				CString championId = lolInfo["championId"].asString().c_str();
				CString companionId = lolInfo["companionId"].asString().c_str();
				CString damageSkinId = lolInfo["damageSkinId"].asString().c_str();
				CString gameId = lolInfo["gameId"].asString().c_str();
				CString gameMode = lolInfo["gameMode"].asString().c_str();
				CString gameQueueType = lolInfo["gameQueueType"].asString().c_str();
				CString gameStatus = lolInfo["gameStatus"].asString().c_str();
				CString iconOverride = lolInfo["iconOverride"].asString().c_str();
				CString isObservable = lolInfo["isObservable"].asString().c_str();
				CString legendaryMasteryScore = lolInfo["legendaryMasteryScore"].asString().c_str();
				CString level = lolInfo["level"].asString().c_str();
				CString mapId = lolInfo["mapId"].asString().c_str();
				CString mapSkinId = lolInfo["mapSkinId"].asString().c_str();
				CString puuid = lolInfo["puuid"].asString().c_str();
				CString queueId = lolInfo["queueId"].asString().c_str();
				CString rankedLeagueDivision = lolInfo["rankedLeagueDivision"].asString().c_str();
				CString rankedLeagueQueue = lolInfo["rankedLeagueQueue"].asString().c_str();
				CString rankedLeagueTier = lolInfo["rankedLeagueTier"].asString().c_str();
				CString rankedLosses = lolInfo["rankedLosses"].asString().c_str();
				CString rankedPrevSeasonDivision = lolInfo["rankedPrevSeasonDivision"].asString().c_str();
				CString rankedPrevSeasonTier = lolInfo["rankedPrevSeasonTier"].asString().c_str();
				CString rankedSplitRewardLevel = lolInfo["rankedSplitRewardLevel"].asString().c_str();
				CString rankedWins = lolInfo["rankedWins"].asString().c_str();
				CString regalia = lolInfo["regalia"].asString().c_str();
				CString skinVariant = lolInfo["skinVariant"].asString().c_str();
				CString skinname = lolInfo["skinname"].asString().c_str();
				CString timeStamp = lolInfo["timeStamp"].asString().c_str();

				item.strGameId = gameId;
			}
		}
		catch(...)
		{
		}
	}

    return item;
}

