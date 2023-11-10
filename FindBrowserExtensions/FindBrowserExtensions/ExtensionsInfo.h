#pragma once


struct AddonInfo
{
	CString strItem;
	CString strDisplayName;
	CString strItemFullPath;
};


class ExtensionsInfo
{
public:
	ExtensionsInfo(void);
	~ExtensionsInfo(void);

	void FindExtension();
	void InitPath();
	void PathExtensionInfo();
	std::vector<AddonInfo> getAddonInfo() const;

private:
	CString GetFolderPath(CString strFolderPath);
	CString GetFolderName(CString strFolderName);

	void searchExtension(CString strDirectory, BOOL bEdge = FALSE);	
	void searchExtensionFireFox(CString strDirectory, BOOL bFirefox = FALSE);	
	CString strEdgeExtension;
	CString strFirefoxExtension;


	void SearchPathAllProfile(CString strDirectory);
	void SearchChromeProfile(CString strDirectory, int nDepth);
	void AddChromeProfile();
	CString strChromeSearchPath1;
	CString strChromeSearchPath2;
	CString strChromeSearchPathProfile;
	CStringArray ArChromeExtensionProfile;


	void SearchPathAllNaverWhaleProfile(CString strDirectory);
	void SearchNaverWhaleProfile(CString strDirectory, int nDepth);
	void AddNaverWhaleProfile();
	void SearchPathNaverWhaleAll(CString strDirectory);
	CString strNaverWhaleSearchPath;
	CStringArray ArNaverWhaleExtensionProfile;


	std::vector<AddonInfo> m_Extensions;
	void AddExtensionInfo(CString strItem, CString strDisplayName, CString strItemFullPath);
	void ClearExtensionInfo();
};
