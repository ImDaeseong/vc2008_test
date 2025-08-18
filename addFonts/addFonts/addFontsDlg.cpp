#include "stdafx.h"
#include "addFonts.h"
#include "addFontsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CaddFontsDlg::CaddFontsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CaddFontsDlg::IDD, pParent)
{
}

void CaddFontsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_fontList);
}

BEGIN_MESSAGE_MAP(CaddFontsDlg, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, &CaddFontsDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CaddFontsDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CaddFontsDlg::OnBnClickedButton3)
END_MESSAGE_MAP()

BOOL CaddFontsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//설치된 폰트 목록 조회
	GetFontList();

	return TRUE;  
}

void CaddFontsDlg::OnPaint()
{
	CPaintDC dc(this);
}

//폰트 설치
void CaddFontsDlg::OnBnClickedButton1()
{
	CString strFontName = _T("RobotoSlab-Regular");
	CString strFontPath = GetModulePath() + _T("\\RobotoSlab-Regular.ttf");

	if (InstallFont(strFontPath, strFontName))
		AfxMessageBox(_T("폰트 설치 성공"), MB_ICONINFORMATION);
	else
		AfxMessageBox(_T("폰트 설치 실패"), MB_ICONERROR);
}

//폰트 삭제
void CaddFontsDlg::OnBnClickedButton3()
{
	CString strFontName = _T("RobotoSlab-Regular");

	if (UnInstallFont(strFontName))
		AfxMessageBox(_T("폰트 삭제 성공"), MB_ICONINFORMATION);
	else
		AfxMessageBox(_T("폰트 삭제 실패"), MB_ICONERROR);
}

//폰트 조회
void CaddFontsDlg::OnBnClickedButton2()
{
	GetFontList();
}

//설치된 폰트 목록
void CaddFontsDlg::GetFontList()
{
    m_fontList.ResetContent();

	//Windows 버전에 따라 레지스트리 키 선택
	HKEY hRootKey = IsWin10Later() ? HKEY_CURRENT_USER : HKEY_LOCAL_MACHINE;

	HKEY hKey;
	if (RegOpenKeyEx(hRootKey, _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts"), 0, KEY_READ, &hKey) != ERROR_SUCCESS)
	{
		AfxMessageBox(_T("폰트 레지스트리를 열 수 없습니다."), MB_ICONERROR);
		return;
	}

	std::vector<CString> fontNames;
	DWORD index = 0;
	TCHAR valueName[512];
	DWORD valueNameSize, dataSize, type;
	BYTE data[512];
	while (true)
	{
		valueNameSize = _countof(valueName);
		dataSize = sizeof(data);

		LONG result = RegEnumValue(hKey, index++, valueName, &valueNameSize, NULL, &type, data, &dataSize);
		if (result == ERROR_NO_MORE_ITEMS) 
			break;

		if (result == ERROR_SUCCESS && type == REG_SZ)
			fontNames.push_back(valueName);
	}

	RegCloseKey(hKey);

	//알파벳 순 정렬
	std::sort(fontNames.begin(), fontNames.end());

	//리스트박스에 추가
	for (size_t i = 0; i < fontNames.size(); ++i)
	{
		m_fontList.AddString(fontNames[i]);
	}
}

//폰트 폴더 경로 
CString CaddFontsDlg::GetFontFolder()
{
	TCHAR path[MAX_PATH] = { 0 };
    if (IsWin10Later())
    {
        if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_CURRENT, path)))
            return CString(path) + _T("\\Microsoft\\Windows\\Fonts");
    }
    else
    {
        if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_FONTS, NULL, SHGFP_TYPE_CURRENT, path)))
            return CString(path);
    }
    return _T("");
}

//폰트 설치
bool CaddFontsDlg::InstallFont(const CString& sFontFilePath, const CString& sFontName)
{    
    if (GetFileAttributes(sFontFilePath) == INVALID_FILE_ATTRIBUTES)
        return false;

    CString fontFolder = GetFontFolder();
    if (fontFolder.IsEmpty())
        return false;

    CString destPath = fontFolder + _T("\\") + sFontName + _T(".ttf");

    if (GetFileAttributes(destPath) != INVALID_FILE_ATTRIBUTES)
        return true;

    if (IsWin10Later())
        SHCreateDirectoryEx(NULL, fontFolder, NULL);

    if (!CopyFile(sFontFilePath, destPath, TRUE))
        return false;

    if (AddFontResourceEx(destPath, 0, NULL) == 0)
    {
        DeleteFile(destPath);
        return false;
    }

    HKEY hRootKey = IsWin10Later() ? HKEY_CURRENT_USER : HKEY_LOCAL_MACHINE;
    HKEY hKey;
    if (RegOpenKeyEx(hRootKey, _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts"), 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS)
    {
        CString regName, regValue;
        regName.Format(_T("%s (TrueType)"), sFontName);
        regValue = IsWin10Later() ? destPath : sFontName + _T(".ttf");
        RegSetValueEx(hKey, regName, 0, REG_SZ, (const BYTE*)(LPCTSTR)regValue, (regValue.GetLength() + 1) * sizeof(TCHAR));
        RegCloseKey(hKey);
    }

    DWORD_PTR result;
    SendMessageTimeout(HWND_BROADCAST, WM_FONTCHANGE, 0, 0, SMTO_ABORTIFHUNG | SMTO_NOTIMEOUTIFNOTHUNG, 1000, &result);

    return true;
}

//폰트 삭제
bool CaddFontsDlg::UnInstallFont(const CString& fontName)
{
	CString fontFolder = GetFontFolder();
    if (fontFolder.IsEmpty())
        return false;

    CString fontPath = fontFolder + _T("\\") + fontName + _T(".ttf");

    if (GetFileAttributes(fontPath) == INVALID_FILE_ATTRIBUTES)
        return true;

    RemoveFontResourceEx(fontPath, 0, NULL);

    HKEY hRootKey = IsWin10Later() ? HKEY_CURRENT_USER : HKEY_LOCAL_MACHINE;
    HKEY hKey;
    if (RegOpenKeyEx(hRootKey, _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts"), 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS)
    {
        CString regName;
        regName.Format(_T("%s (TrueType)"), fontName);
        RegDeleteValue(hKey, regName);
        RegCloseKey(hKey);
    }

    Sleep(500);
    DeleteFile(fontPath);

    DWORD_PTR result;
    SendMessageTimeout(HWND_BROADCAST, WM_FONTCHANGE, 0, 0, SMTO_ABORTIFHUNG | SMTO_NOTIMEOUTIFNOTHUNG, 1000, &result);

    return true;
}

CString CaddFontsDlg::GetModulePath(LPCTSTR subPath)
{
	TCHAR buf[MAX_PATH] = { 0 };
	if (::GetModuleFileName(NULL, buf, MAX_PATH) == 0) return _T("");

	CString dir = buf;
	int pos = dir.ReverseFind(_T('\\'));
	if (pos >= 0) dir = dir.Left(pos);

	if (subPath && subPath[0] != _T('\0'))
	{
		if (subPath[0] == _T('\\')) return dir + subPath;
		return dir + _T("\\") + subPath;
	}
	return dir;
}

//Windows 10 이상 여부
bool CaddFontsDlg::IsWin10Later()
{
	typedef LONG(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);
    HMODULE hMod = GetModuleHandle(_T("ntdll.dll"));
    if (hMod)
    {
        RtlGetVersionPtr fxPtr = (RtlGetVersionPtr)GetProcAddress(hMod, "RtlGetVersion");
        if (fxPtr)
        {
            RTL_OSVERSIONINFOW rovi = { 0 };
            rovi.dwOSVersionInfoSize = sizeof(rovi);
            if (fxPtr(&rovi) == 0)
                return rovi.dwMajorVersion >= 10;
        }
    }
    return false;
}