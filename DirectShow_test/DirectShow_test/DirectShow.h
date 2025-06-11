#pragma once
#include "dshow.h"
#pragma comment(lib, "Strmiids.lib")

class DirectShow
{
public:
	DirectShow();
	~DirectShow();

	void init(OAHWND hWnd);
    void Clear();
    void Load(CString strFileName);
    void Run();
    void Pause();
    void Stop();
    void FullScreen(bool bFull);

    void SetPosition(int x, int y, int nWidth, int nHeight);

    void setCurrentPosition(REFERENCE_TIME pos);
    REFERENCE_TIME getCurrentPosition();
    REFERENCE_TIME getLength();
    void setZero();
    void seek(REFERENCE_TIME* rtTotal, REFERENCE_TIME* rtNow);

    long GetIt(UINT wparam, LONG lparam);
    void setNotifyWindow(UINT msg);

	IMediaSeeking* GetSeek() { return pSeek; }

private:
	IGraphBuilder* pGraph;
    IMediaControl* pMediaControl;
    IMediaEventEx* pEvent;
    IVideoWindow* pVidWin;
    IMediaSeeking* pSeek;

    REFERENCE_TIME rtTotal;
    OAHWND m_hWnd;
};
