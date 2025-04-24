#pragma once

struct TextInfo
{
    CString strText;
    int nFontSize;
    D3DCOLOR clrColor;
    int nFontWeight; // FW_BOLD or FW_NORMAL
};

class COverlayWnd : public CWnd
{
public:
	COverlayWnd();
	~COverlayWnd();
	
	void addText(const std::vector<TextInfo>& vLine);

protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnDestroy();
    afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()

private:
	std::vector<TextInfo> m_vLine;


	LPDIRECT3D9             m_pD3D;
    LPDIRECT3DDEVICE9       m_pD3DDevice;
    LPDIRECT3DTEXTURE9      m_pBackgroundTexture;
    LPD3DXSPRITE            m_pSprite;
	bool                    m_bInitialized;

    void InitializeDirectX();
	void InitializeD3DDevice();
    void DrawTextOverlay();
    void Cleanup();
    void RenderBackground();


	CString m_strPath;
	void LoadBackgroundTextureFromFile(const CString& strPath);
	void LoadBackgroundTextureFromResource(UINT nResourceID, LPCTSTR lpResourceType);
};
