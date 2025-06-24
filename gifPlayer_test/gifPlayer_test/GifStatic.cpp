#include "StdAfx.h"
#include "GifStatic.h"

CGifStatic::CGifStatic()
{
    m_pGifImage = NULL;
    m_nCurrentFrame = 0;
    m_nFrameCount = 0;
    m_nTimer = 0;
    m_bInitialized = FALSE;
}

CGifStatic::~CGifStatic()
{
    Stop();
    CleanupGifImage();
}

BEGIN_MESSAGE_MAP(CGifStatic, CStatic)
    ON_WM_TIMER()    
    ON_WM_PAINT()    
    ON_WM_DESTROY()  
END_MESSAGE_MAP()

BOOL CGifStatic::LoadGIF(LPCTSTR lpszFileName)
{
    Stop();
    CleanupGifImage();

    if (!lpszFileName || _tcslen(lpszFileName) == 0)
        return FALSE;

    DWORD dwAttrib = GetFileAttributes(lpszFileName);
    if (dwAttrib == INVALID_FILE_ATTRIBUTES)
        return FALSE;
    
    try
    {
        m_pGifImage = Gdiplus::Image::FromFile(CA2W(lpszFileName));
    }
    catch (...)
    {
        CleanupGifImage();
        return FALSE;
    }

    if (!m_pGifImage || m_pGifImage->GetLastStatus() != Gdiplus::Ok)
    {
        CleanupGifImage();
        return FALSE;
    }

    UINT count = m_pGifImage->GetFrameCount(&Gdiplus::FrameDimensionTime);
    if (count == 0)
    {
        CleanupGifImage();
        return FALSE;
    }

    m_nFrameCount = count;      
    m_nCurrentFrame = 0;        

    if (!GetGifFrameDelays())
    {
        CleanupGifImage();
        return FALSE;
    }

    m_bInitialized = TRUE; 

    return TRUE;
}

BOOL CGifStatic::GetGifFrameDelays()
{
    if (!m_pGifImage)
        return FALSE;

    m_frameDelays.clear(); 

    try
    {
        UINT size = m_pGifImage->GetPropertyItemSize(PropertyTagFrameDelay);
        if (size > 0)
        {
            PropertyItem* pItem = (PropertyItem*)malloc(size);
            if (!pItem)
                return FALSE;

            if (m_pGifImage->GetPropertyItem(PropertyTagFrameDelay, size, pItem) == Gdiplus::Ok)
            {
                UINT* pDelay = (UINT*)pItem->value;
                for (UINT i = 0; i < m_nFrameCount; i++)
                {
                    UINT delay = pDelay[i] * 10;
                    if (delay == 0)
					{
                        delay = 100; 
					}
                    m_frameDelays.push_back(delay); 
                }
            }

            free(pItem); 
        }
    }
    catch (...)
    {
    }

    while (m_frameDelays.size() < m_nFrameCount)
    {
        m_frameDelays.push_back(100); 
    }

    return TRUE;
}

void CGifStatic::Start()
{
    if (!m_bInitialized || !m_pGifImage || m_frameDelays.empty() || m_nFrameCount <= 1)
        return;

    Stop();

    m_nCurrentFrame = 0;

    if (m_pGifImage->SelectActiveFrame(&Gdiplus::FrameDimensionTime, m_nCurrentFrame) == Gdiplus::Ok)
    {
        m_nTimer = SetTimer(1, m_frameDelays[m_nCurrentFrame], NULL);
        Invalidate(); 
    }
}

void CGifStatic::Stop()
{
    if (m_nTimer != 0)
    {
        KillTimer(m_nTimer); 
        m_nTimer = 0;        
    }
}

void CGifStatic::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent != 1 || !m_bInitialized || !m_pGifImage || m_frameDelays.empty())
        return;

    if (m_nTimer != 0)
    {
        KillTimer(m_nTimer);
        m_nTimer = 0;
    }

    m_nCurrentFrame = (m_nCurrentFrame + 1) % m_nFrameCount;

    try
    {
        if (m_pGifImage->SelectActiveFrame(&Gdiplus::FrameDimensionTime, m_nCurrentFrame) == Gdiplus::Ok)
        {
	        Invalidate(); 

            m_nTimer = SetTimer(1, m_frameDelays[m_nCurrentFrame], NULL);
        }
        else
        {
            Stop();
        }
    }
    catch (...)
    {
        Stop();
    }

    CStatic::OnTimer(nIDEvent); 
}

void CGifStatic::OnPaint()
{
    CPaintDC dc(this); 

    if (!m_bInitialized || !m_pGifImage)
    {
        CRect rect;
        GetClientRect(&rect); 
        dc.FillSolidRect(&rect, GetSysColor(COLOR_BTNFACE)); 
        return;
    }

    try
    {
        Gdiplus::Graphics graphics(dc.GetSafeHdc()); 
        graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
        graphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);

        CRect rect;
        GetClientRect(&rect); 

        graphics.DrawImage(m_pGifImage, Gdiplus::Rect(rect.left, rect.top, rect.Width(), rect.Height()));
    }
    catch (...)
    {
        CRect rect;
        GetClientRect(&rect);
        dc.FillSolidRect(&rect, GetSysColor(COLOR_BTNFACE));
    }
}

void CGifStatic::OnDestroy()
{
    Stop();               
    CleanupGifImage();    

    CStatic::OnDestroy(); 
}

void CGifStatic::CleanupGifImage()
{
    if (m_pGifImage)
    {
        try
        {
            delete m_pGifImage; 
        }
        catch (...)
        {
        }
        m_pGifImage = NULL; 
    }

    m_frameDelays.clear(); 
    m_nFrameCount = 0;     
    m_nCurrentFrame = 0;   
    m_bInitialized = FALSE; 
}