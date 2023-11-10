#include "StdAfx.h"
#include "ExtensionsInfo.h"

ExtensionsInfo::ExtensionsInfo(void)
{
}

ExtensionsInfo::~ExtensionsInfo(void)
{
}

void ExtensionsInfo::FindExtension()
{
	ClearExtensionInfo();

	searchExtension(strChromeSearchPath1);
	searchExtension(strChromeSearchPath2);
	searchExtension(strEdgeExtension, TRUE);
	searchExtensionFireFox(strFirefoxExtension, TRUE);
	SearchPathAllProfile(strChromeSearchPathProfile);
	SearchPathAllNaverWhaleProfile(strNaverWhaleSearchPath);
}

void ExtensionsInfo::InitPath()
{

	RegInfo reginfo;
	CString strAppData = reginfo.GetAppDataPath();
	CString strLocalAppData = reginfo.GetLocalAppDataPath();

	//Chrome1
	strChromeSearchPath1.Format(_T("%s\\Google\\Chrome\\User Data\\Default\\Extensions"), strLocalAppData);

	//Chrome2
	strChromeSearchPath2.Format(_T("%s\\Google\\Chrome\\User Data\\Default\\Sync Extension Settings"), strLocalAppData);

	//Chrome3
	strChromeSearchPathProfile.Format(_T("%s\\Google\\Chrome\\User Data"), strLocalAppData);
	
	//Edge
	strEdgeExtension.Format(_T("%s\\Microsoft\\Edge\\User Data\\Default\\Extensions"), strLocalAppData);

	//Firefox
	strFirefoxExtension.Format(_T("%s\\Mozilla\\Firefox\\Profiles\\"), strAppData);

	//naver whale
	strNaverWhaleSearchPath.Format(_T("%s\\Naver\\Naver Whale\\User Data"), strLocalAppData);
}

void ExtensionsInfo::PathExtensionInfo()
{
	OutputDebugString(strChromeSearchPath1 + "\n");
	OutputDebugString(strChromeSearchPath2 + "\n");
	OutputDebugString(strChromeSearchPathProfile + "\n");
	OutputDebugString(strEdgeExtension + "\n");
	OutputDebugString(strFirefoxExtension + "\n");
	OutputDebugString(strNaverWhaleSearchPath + "\n");
}

std::vector<AddonInfo> ExtensionsInfo::getAddonInfo() const
{
    return m_Extensions;
}

CString ExtensionsInfo::GetFolderPath(CString strFolderPath)
{
	int nPos = strFolderPath.ReverseFind('\\');
	if (nPos > 0)
		return strFolderPath.Left(nPos);

	return strFolderPath;
}

CString ExtensionsInfo::GetFolderName(CString strFolderName)
{
	int nPos = strFolderName.ReverseFind('\\');
	if (nPos > 0)
		return strFolderName.Mid(nPos + 1);

	return strFolderName;
}

void ExtensionsInfo::searchExtension(CString strDirectory, BOOL bEdge)
{
	CString strFilter = strDirectory + _T("\\*.*");

	double start = 0;
	double stop = 0;

	CFileFind finder;
	BOOL bSearch = finder.FindFile(strFilter, 0);

	while (bSearch)
	{
		bSearch = finder.FindNextFile();

		if (finder.IsDots())
			continue;

		if (finder.IsDirectory())
		{
			CString strItem = GetFolderName(finder.GetFilePath());

			if (bEdge)
				AddExtensionInfo(strItem, _T("edge"), finder.GetFilePath());
			else
				AddExtensionInfo(strItem, _T("chrome"), finder.GetFilePath());
		}		
	}
	finder.Close();
}

void ExtensionsInfo::searchExtensionFireFox(CString strDirectory, BOOL bFirefox)
{
	CFileFind file;
	BOOL bFind = file.FindFile(strDirectory + _T("\\*.*"));
	CString strFolderItem, strFileExit, strTemp;

	while (bFind)
	{
		bFind = file.FindNextFile();
		if (file.IsDirectory() && !file.IsDots())
		{
			strFolderItem = file.GetFilePath();

			CString strExetension = GetFolderName(strFolderItem);
			if (lstrcmpi(strExetension, _T("extensions")) == 0)
			{
				searchExtensionFireFox(strFolderItem, true);
			}
			else
			{
				searchExtensionFireFox(strFolderItem, false);
			}
		}

		strFolderItem = file.GetFileName();
		if (!file.IsDots())
		{
			if (file.IsDirectory())
				continue;

			if (bFirefox)
			{
				CString strFileName;
				strFileName = strFolderItem.MakeLower();
				AddExtensionInfo(strFileName, _T("firefox"), file.GetFilePath());
			}
		}
	}
}

void ExtensionsInfo::SearchPathAllProfile(CString strDirectory)
{
	SearchChromeProfile(strDirectory, 0);
	AddChromeProfile();
}

void ExtensionsInfo::SearchChromeProfile(CString strDirectory, int nDepth)
{
	CString strFilter = strDirectory + _T("\\*.*");

	CFileFind finder;
	BOOL bSearch = finder.FindFile(strFilter, 0);
	while (bSearch)
	{
		bSearch = finder.FindNextFile();

		if (finder.IsDots())
			continue;

		if (finder.IsDirectory())
		{
			if (nDepth > 0)
				continue;

			CString szFolderPath = finder.GetFilePath();

			CString strTemp = szFolderPath.MakeLower();
			if (strTemp.Find(_T("profile")) > 0)
			{
				ArChromeExtensionProfile.Add(szFolderPath);
				SearchChromeProfile(szFolderPath, nDepth + 1);
			}
		}
	}
	finder.Close();
}

void ExtensionsInfo::AddChromeProfile()
{
	CString strFodler1, strFodler2;
	for (int i = 0; i < ArChromeExtensionProfile.GetSize(); i++)
	{
		strFodler1.Format(_T("%s\\Extensions"), ArChromeExtensionProfile.GetAt(i));
		strFodler2.Format(_T("%s\\Sync Extension Settings"), ArChromeExtensionProfile.GetAt(i));
		searchExtension(strFodler1);
		searchExtension(strFodler2);
	}
}

void ExtensionsInfo::SearchPathAllNaverWhaleProfile(CString strDirectory)
{
	ArNaverWhaleExtensionProfile.Add(strDirectory + CString("\\Default"));

	SearchNaverWhaleProfile(strDirectory, 0);
	AddNaverWhaleProfile();
}

void ExtensionsInfo::SearchNaverWhaleProfile(CString strDirectory, int nDepth)
{
	CString strFilter = strDirectory + _T("\\*.*");

	CFileFind finder;
	BOOL bSearch = finder.FindFile(strFilter, 0);
	while (bSearch)
	{
		bSearch = finder.FindNextFile();

		if (finder.IsDots())
			continue;

		if (finder.IsDirectory())
		{
			if (nDepth > 0)
				continue;

			CString szFolderPath = finder.GetFilePath();

			CString strTemp = szFolderPath.MakeLower();
			if (strTemp.Find(_T("profile")) > 0)
			{
				ArNaverWhaleExtensionProfile.Add(szFolderPath);

				SearchNaverWhaleProfile(szFolderPath, nDepth + 1);
			}
		}
	}
	finder.Close();
}

void ExtensionsInfo::AddNaverWhaleProfile()
{
	CString strFodler1, strFodler2;
	for (int i = 0; i < ArNaverWhaleExtensionProfile.GetSize(); i++)
	{
		strFodler1.Format(_T("%s\\Extensions"), ArNaverWhaleExtensionProfile.GetAt(i));
		strFodler2.Format(_T("%s\\Sync Extension Settings"), ArNaverWhaleExtensionProfile.GetAt(i));

		SearchPathNaverWhaleAll(strFodler1);
		SearchPathNaverWhaleAll(strFodler2);
	}
}

void ExtensionsInfo::SearchPathNaverWhaleAll(CString strDirectory)
{
	CString strFilter = strDirectory + _T("\\*.*");

	double start = 0;
	double stop = 0;

	CFileFind finder;
	BOOL bSearch = finder.FindFile(strFilter, 0);

	while (bSearch)
	{
		bSearch = finder.FindNextFile();

		if (finder.IsDots())
			continue;

		if (finder.IsDirectory())
		{
			CString strItem = GetFolderName(finder.GetFilePath());

			AddExtensionInfo(strItem, _T("NaverWhale"), finder.GetFilePath());
		}
	}
	finder.Close();
}

void ExtensionsInfo::AddExtensionInfo(CString strItem, CString strDisplayName, CString strItemFullPath)
{	
	AddonInfo info;
	info.strItem = strItem;
	info.strDisplayName = strDisplayName;
	info.strItemFullPath = strItemFullPath;
	m_Extensions.push_back(info);
}

void ExtensionsInfo::ClearExtensionInfo()
{
	m_Extensions.clear();

	ArChromeExtensionProfile.RemoveAll();

	ArNaverWhaleExtensionProfile.RemoveAll();
}