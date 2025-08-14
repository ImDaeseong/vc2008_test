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

void CaddFontsDlg::OnBnClickedButton1()
{
	CString sFontFilePath;
	sFontFilePath.Format("..\\RobotoSlab-Regular.ttf");

	CString sFontName = "RobotoSlab-Regular";

	//폰트 설치
	InstallFont(sFontFilePath, sFontName);

	//설치된 폰트 목록 조회
	GetFontList();
}

//설치된 폰트 목록
void CaddFontsDlg::GetFontList()
{
	m_fontList.ResetContent();

	HKEY hKey;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts"), 0, KEY_READ, &hKey) != ERROR_SUCCESS)
	{
		AfxMessageBox(_T("폰트 레지스트리를 열 수 없습니다."), MB_ICONERROR);
		return;
	}

	std::vector<CString> fontNames;

	DWORD index = 0;
	TCHAR valueName[512];
	DWORD valueNameSize, dataSize, type;
	BYTE data[512];
	LONG result;

	while (true)
	{
		valueNameSize = _countof(valueName);
		dataSize = sizeof(data);
	    
		result = RegEnumValue(hKey, index++, valueName, &valueNameSize, NULL, &type, data, &dataSize);
	    
		if (result == ERROR_NO_MORE_ITEMS)
			break;
	    
		if (result == ERROR_SUCCESS && type == REG_SZ)
		{
			fontNames.push_back(CString(valueName));
		}
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

//폰트 설치
BOOL CaddFontsDlg::InstallFont(const CString& sFontFilePath, const CString& sFontName)
{    
    //폰트 파일 존재 확인
    if (GetFileAttributes(sFontFilePath) == INVALID_FILE_ATTRIBUTES)
    {
        return FALSE;
    }

    //Fonts 폴더 경로 얻기(C:\Windows\Fonts)
    TCHAR tPath[MAX_PATH] = {0};
    HRESULT hr = SHGetFolderPath(NULL, CSIDL_FONTS, NULL, SHGFP_TYPE_CURRENT, tPath);
    if (FAILED(hr))
    {
        return FALSE;
    }

    //설치할 폰트
    CString strMyFontPath;
    strMyFontPath.Format(_T("%s\\%s.ttf"), tPath, (LPCTSTR)sFontName);

    //이미 설치된 폰트인지 확인
    if (GetFileAttributes(strMyFontPath) != INVALID_FILE_ATTRIBUTES)
    {
        //이미 설치됨
        return TRUE;  
    }

    //폰트 파일 복사
    if (!CopyFile(sFontFilePath, strMyFontPath, TRUE))
    {
        return FALSE;
    }

    //AddFontResourceEx로 폰트 로드
    int fontCount = AddFontResourceEx(strMyFontPath, FR_PRIVATE, NULL);
    if (fontCount == 0)
    {
        // 복사한 파일 정리
        DeleteFile(strMyFontPath);
        return FALSE;
    }

    //레지스트리 등록
    HKEY hKey = NULL;
    LONG regResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts"), 0, KEY_SET_VALUE, &hKey);
    
    if (regResult == ERROR_SUCCESS)
    {
        CString sRegName;
		CString sRegValue;

        sRegName.Format(_T("%s (TrueType)"), (LPCTSTR)sFontName);
        sRegValue.Format(_T("%s.ttf"), (LPCTSTR)sFontName);
        
        DWORD lValuseSize = (sRegValue.GetLength() + 1) * sizeof(TCHAR);
        regResult = RegSetValueEx(hKey, sRegName, 0, REG_SZ, (const BYTE*)(LPCTSTR)sRegValue, lValuseSize);
        
        if (regResult != ERROR_SUCCESS)
        {
            _tprintf(_T("레지스트리 값 설정 실패 (오류코드: %d)\n"), regResult);
        }
        
        RegCloseKey(hKey);
    }
    
    //시스템에 폰트 변경 알림	
	DWORD_PTR result;
	if (SendMessageTimeout(HWND_BROADCAST, WM_FONTCHANGE, 0, 0, SMTO_ABORTIFHUNG | SMTO_NOTIMEOUTIFNOTHUNG, 1000, &result) == 0)
	{
		::PostMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);
	}

    return TRUE;
}
