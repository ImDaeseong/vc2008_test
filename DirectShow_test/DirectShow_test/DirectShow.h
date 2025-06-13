#pragma once
#include <dshow.h>
#pragma comment(lib, "Strmiids.lib")

class DirectShow 
{
public:
    DirectShow();
    ~DirectShow();

    void init(OAHWND hWnd);
    void Load(LPCTSTR strFileName);
    void Run();
    void Pause();
    void Stop();
    void FullScreen(bool bFull);
    void Clear();

    void SetPosition(int x, int y, int nWidth, int nHeight);

    void setCurrentPosition(REFERENCE_TIME pos);
    REFERENCE_TIME getCurrentPosition();
    REFERENCE_TIME getLength();
    void setZero();
    void seek(REFERENCE_TIME* rtTotal, REFERENCE_TIME* rtNow);

    long HandleEvent(UINT wparam, LONG lparam);
    void SetNotifyWindow(UINT msg);

    void setVolume(long volume);         // 0 (최대) ~ -10000 (음소거)
    long getVolume() const;

    //오디오 스트림 선택
    int getAudioStreamCount() const;
    void selectAudioStream(int index);

	CComPtr<IMediaSeeking> GetSeek();

private:
    CComPtr<IGraphBuilder> pGraph;
    CComPtr<IMediaControl> pMediaControl;
    CComPtr<IMediaEventEx> pEvent;
    CComPtr<IVideoWindow> pVidWin;
    CComPtr<IMediaSeeking> pSeek;
    CComPtr<IBasicAudio> pAudio;
    CComPtr<IAMStreamSelect> pStreamSelect;

    REFERENCE_TIME rtTotal;
    OAHWND m_hWnd;
};
