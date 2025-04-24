#pragma once
#include "StdAfx.h"
#include "OverlayWnd.h"
#include "resource.h"

COverlayWnd::COverlayWnd()
{
    m_pD3D = NULL;
    m_pD3DDevice = NULL;
    m_pBackgroundTexture = NULL;
	m_pSprite = NULL;
    m_bInitialized = false;

	m_strPath = _T("");
}

COverlayWnd::~COverlayWnd()
{
    m_vLine.clear();

    Cleanup();
}

BEGIN_MESSAGE_MAP(COverlayWnd, CWnd)
    ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_WM_PAINT()
END_MESSAGE_MAP()

int COverlayWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    InitializeDirectX();

	//배경이미지 
    //LoadBackgroundTextureFromFile(_T("c:\\spritebox.png"));

	//배경이미지 리소스
	LoadBackgroundTextureFromResource(IDB_PNG1, _T("PNG"));

    return 0;
}

void COverlayWnd::OnDestroy()
{
    Cleanup();

    CWnd::OnDestroy();
}

void COverlayWnd::OnPaint()
{
    CPaintDC dc(this);

    DrawTextOverlay();
}

//Direct3D 초기화
void COverlayWnd::InitializeDirectX()
{
    if (m_bInitialized) return;

	//Direct3D 객체 생성
    m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (!m_pD3D) return;

	//Direct3D 디바이스 파라미터 설정
    D3DPRESENT_PARAMETERS d3dpp = {};
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = m_hWnd;
    d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;

	//Direct3D 디바이스 생성
    if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice))) 
    {
        return;
    }

	//알파 블렌딩 설정
    m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//스프라이트 객체 생성
    D3DXCreateSprite(m_pD3DDevice, &m_pSprite);

    m_bInitialized = true;
}

//D3D 장치 초기화
void COverlayWnd::InitializeD3DDevice()
{
	if (!m_pD3DDevice) return;

    //기존 텍스처 해제
    if (m_pBackgroundTexture)
    {
        m_pBackgroundTexture->Release();
        m_pBackgroundTexture = NULL;
    }
}

//텍스트 및 배경 렌더링 
void COverlayWnd::DrawTextOverlay()
{
    if (!m_bInitialized || !m_pD3DDevice) return;

	if (!m_pBackgroundTexture)
    {
		//텍스처 재생성 
        InitializeD3DDevice();
    }

    CRect clientRect;
    GetClientRect(&clientRect);

	//화면 클리어
    m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
    m_pD3DDevice->BeginScene();

    //배경 렌더링
    RenderBackground();
     
	int nOffset = 10;
    for (size_t i = 0; i < m_vLine.size(); ++i)
    {
        const TextInfo& info = m_vLine[i];

        LPD3DXFONT pFont = NULL;
        if (SUCCEEDED(D3DXCreateFont(m_pD3DDevice, info.nFontSize, 0, info.nFontWeight, 1, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH,
            _T("돋움"), &pFont)))
        {
            RECT rect = {0, nOffset, clientRect.Width(), nOffset + info.nFontSize + 10 };
            pFont->DrawText(NULL, info.strText, -1, &rect, DT_CENTER | DT_NOCLIP, info.clrColor);
            pFont->Release();
        }

        nOffset += info.nFontSize + 10;
    }

    m_pD3DDevice->EndScene();

	//최종 렌더링
    m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//Direct3D 리소스 정리 
void COverlayWnd::Cleanup()
{
	if (m_pBackgroundTexture) 
	{
        m_pBackgroundTexture->Release();
        m_pBackgroundTexture = NULL;
    }

    if (m_pSprite) 
	{
        m_pSprite->Release();
        m_pSprite = NULL;
    }

    if (m_pD3DDevice)
    {
        m_pD3DDevice->Release();
        m_pD3DDevice = NULL;
    }

    if (m_pD3D)
    {
        m_pD3D->Release();
        m_pD3D = NULL;
    }

    m_bInitialized = false;
}

//배경 이미지 렌더링
void COverlayWnd::RenderBackground()
{
    if (!m_pBackgroundTexture || !m_pSprite) return;

    m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

    D3DSURFACE_DESC desc;
    m_pBackgroundTexture->GetLevelDesc(0, &desc);
    float texWidth = static_cast<float>(desc.Width);
    float texHeight = static_cast<float>(desc.Height);

    CRect rcClient;
    GetClientRect(&rcClient);
    float screenWidth = static_cast<float>(rcClient.Width());
    float screenHeight = static_cast<float>(rcClient.Height());

    D3DXVECTOR3 pos((screenWidth - texWidth) / 2.0f, (screenHeight - texHeight) / 2.0f, 0);
    m_pSprite->Draw(m_pBackgroundTexture, NULL, NULL, &pos, D3DCOLOR_ARGB(255, 255, 255, 255));

    m_pSprite->End();
}

//이미지 경로
void COverlayWnd::LoadBackgroundTextureFromFile(const CString& strPath)
{	 
	m_strPath = strPath;

	if (!m_bInitialized || !m_pD3DDevice)
		return;

	//기존 텍스처 해제
	if (m_pBackgroundTexture)
	{
		m_pBackgroundTexture->Release();
		m_pBackgroundTexture = NULL;
	}

	//새로운 텍스처 생성
	HRESULT hr = D3DXCreateTextureFromFile(m_pD3DDevice, m_strPath, &m_pBackgroundTexture);
	if (FAILED(hr)) return;
}

//리소스
void COverlayWnd::LoadBackgroundTextureFromResource(UINT nResourceID, LPCTSTR lpResourceType)
{
    if (!m_bInitialized || !m_pD3DDevice)
        return;

    if (m_pBackgroundTexture)
    {
        m_pBackgroundTexture->Release();
        m_pBackgroundTexture = NULL;
    }

    HMODULE hModule = AfxGetResourceHandle();
    HRSRC hResource = FindResource(hModule, MAKEINTRESOURCE(nResourceID), lpResourceType);
    if (!hResource) return;
    
    DWORD imageSize = SizeofResource(hModule, hResource);
    if (imageSize == 0) return;
    
    HGLOBAL hLoadedResource = LoadResource(hModule, hResource);
    if (!hLoadedResource) return;
    
    const void* pResourceData = LockResource(hLoadedResource);
    if (!pResourceData) return;
    
    HRESULT hr = D3DXCreateTextureFromFileInMemory(m_pD3DDevice, pResourceData, imageSize, &m_pBackgroundTexture);
    if (FAILED(hr)) return;    
}

void COverlayWnd::addText(const std::vector<TextInfo>& vLine)
{
    m_vLine = vLine;
	//Invalidate();
}