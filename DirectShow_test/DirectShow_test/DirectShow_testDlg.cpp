#include "stdafx.h"
#include "DirectShow_test.h"
#include "DirectShow_testDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static UINT WM_GRAPHNOTIFY = RegisterWindowMessage(_T("GRAPHNOTIFY"));

CDirectShow_testDlg::CDirectShow_testDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDirectShow_testDlg::IDD, pParent)
{
}

void CDirectShow_testDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDirectShow_testDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_HSCROLL()
	ON_REGISTERED_MESSAGE(WM_GRAPHNOTIFY, OnGraphNotify)
	ON_BN_CLICKED(IDC_BUTTON1, &CDirectShow_testDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDirectShow_testDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDirectShow_testDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDirectShow_testDlg::OnBnClickedButton4)
END_MESSAGE_MAP()

BOOL CDirectShow_testDlg::DestroyWindow()
{
	KillTimer(1);
	m_DirectShow.Clear();

	return CDialog::DestroyWindow();
}

void CDirectShow_testDlg::OnTimer(UINT_PTR nIDEvent)
{
	IMediaSeeking* pSeek = m_DirectShow.GetSeek();
	if (pSeek)
	{
		REFERENCE_TIME rtTotal = 0, rtNow = 0;

		if (SUCCEEDED(pSeek->GetDuration(&rtTotal)) && SUCCEEDED(pSeek->GetCurrentPosition(&rtNow)) && rtTotal > 0)
		{
			int minutes = (int)((rtNow / 10000000LL) / 60);
			int seconds = (int)((rtNow / 10000000LL) % 60);
			int percent = (int)((rtNow * 100) / rtTotal);

			CString strMsg;
			strMsg.Format(_T("재생 중: %02d:%02d (%d%%)"), minutes, seconds, percent);
			GetDlgItem(IDC_STATIC)->SetWindowText(strMsg);

			// 슬라이더 위치를 1초 단위로 설정 (슬라이더 최대 범위 설정도 필요)
			CSliderCtrl* pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER1);
			pSlider->SetRange(0, (int)(rtTotal / 10000000LL));
			pSlider->SetPos((int)(rtNow / 10000000LL));
		}
		else
		{
			// 재생 중이 아니거나 오류 시 초기화
			GetDlgItem(IDC_STATIC)->SetWindowText(_T("재생 중: 00:00 (0%)"));
			((CSliderCtrl*)GetDlgItem(IDC_SLIDER1))->SetPos(0);
		}
	}
	else
	{
		GetDlgItem(IDC_STATIC)->SetWindowText(_T("재생 중: 00:00 (0%)"));
		((CSliderCtrl*)GetDlgItem(IDC_SLIDER1))->SetPos(0);
	}

	CDialog::OnTimer(nIDEvent);
}

void CDirectShow_testDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSliderCtrl* pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER1);

	if (pScrollBar && pSlider && pScrollBar->GetSafeHwnd() == pSlider->GetSafeHwnd())
	{
		// 사용자가 마우스 버튼을 놓았을 때만 Seek 호출
		if (nSBCode == TB_ENDTRACK || nSBCode == SB_ENDSCROLL)
		{
			REFERENCE_TIME pos = (REFERENCE_TIME)pSlider->GetPos() * 10000000LL;
			m_DirectShow.setCurrentPosition(pos);
		}
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

LRESULT CDirectShow_testDlg::OnGraphNotify(WPARAM wp, LPARAM lp)
{
	long evCode = m_DirectShow.GetIt((UINT)wp, (LONG)lp);
	if (evCode == EC_COMPLETE)
	{
		GetDlgItem(IDC_STATIC)->SetWindowText(_T("재생 완료"));
		((CSliderCtrl*)GetDlgItem(IDC_SLIDER1))->SetPos(0);
	}

    return 0;
}

BOOL CDirectShow_testDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_DirectShow.init((OAHWND)GetSafeHwnd());
	
	SetTimer(1, 1000, 0);

	return TRUE;  
}

void CDirectShow_testDlg::OnPaint()
{
	CPaintDC dc(this);
}

void CDirectShow_testDlg::OnBnClickedButton1()
{
	m_DirectShow.Clear();
	m_DirectShow.init((OAHWND)GetSafeHwnd());
	m_DirectShow.setNotifyWindow(WM_GRAPHNOTIFY);
	m_DirectShow.Load(_T("aa.avi"));

	//https 미지원
	//mp4 실행시 코덱 필요
}

void CDirectShow_testDlg::OnBnClickedButton2()
{
	m_DirectShow.Stop();
}

void CDirectShow_testDlg::OnBnClickedButton3()
{
	m_DirectShow.Run();
}

void CDirectShow_testDlg::OnBnClickedButton4()
{
	m_DirectShow.FullScreen(TRUE);
}
