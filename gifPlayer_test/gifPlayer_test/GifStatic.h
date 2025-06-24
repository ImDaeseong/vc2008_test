#pragma once

class CGifStatic : public CStatic
{
public:
    CGifStatic();
    virtual ~CGifStatic();

    BOOL LoadGIF(LPCTSTR lpszFileName);
    void Start();
    void Stop();

protected:
    Gdiplus::Image* m_pGifImage;       
    UINT m_nCurrentFrame;              
    UINT m_nFrameCount;                
    UINT_PTR m_nTimer;                 
    std::vector<UINT> m_frameDelays;   
    BOOL m_bInitialized;               

    BOOL GetGifFrameDelays();
    void CleanupGifImage();

protected:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnPaint();
	afx_msg void OnDestroy();
};