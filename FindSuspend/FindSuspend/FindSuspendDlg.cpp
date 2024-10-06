#include "stdafx.h"
#include "FindSuspend.h"
#include "FindSuspendDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CFindSuspendDlg::CFindSuspendDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFindSuspendDlg::IDD, pParent)
{
}

void CFindSuspendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}

BEGIN_MESSAGE_MAP(CFindSuspendDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CFindSuspendDlg::OnNMClickList1)
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CFindSuspendDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitList();

	RefreshProcessList();
    
    SetTimer(1, 5000, NULL);

	return TRUE;  
}

void CFindSuspendDlg::OnPaint()
{
	CPaintDC dc(this);
}

void CFindSuspendDlg::OnDestroy()
{
	KillTimer(1);
	CDialog::OnDestroy();
}

void CFindSuspendDlg::InitList()
{
	m_ListCtrl.DeleteAllItems();
	m_ListCtrl.InsertColumn(0, _T("프로세스명"), LVCFMT_CENTER, 310, -1);
	m_ListCtrl.InsertColumn(1, _T("상태"), LVCFMT_LEFT, 150, -1);
}

void CFindSuspendDlg::InsertList(const CString& strProcess, const CString& strState, DWORD processID)
{
    int nIndex = m_ListCtrl.GetItemCount();
    m_ListCtrl.InsertItem(nIndex, strProcess);
    m_ListCtrl.SetItemText(nIndex, 1, strState);
    m_ProcessIDs.push_back(processID);
}

void CFindSuspendDlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    int nItem = pNMItemActivate->iItem;
    
    if (nItem >= 0)
    {
        ToggleProcessState(nItem);
    }
    
    *pResult = 0;
}

void CFindSuspendDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
    {
		RefreshProcessList();

        for (int i = 0; i < m_ListCtrl.GetItemCount(); ++i)
        {
            UpdateProcessState(i);
        }
    }

	CDialog::OnTimer(nIDEvent);
}

void CFindSuspendDlg::RefreshProcessList()
{    
	m_ListCtrl.DeleteAllItems();
    m_ProcessIDs.clear();

	const char* files[] = {"chrome.exe", "explorer.exe", "msedge.exe", "whale.exe", "firefox.exe"};

	for (int i = 0; i < sizeof(files) / sizeof(files[0]); i++)
	{
		std::vector<ProcessInfo> processes = m_CheckSuspend.GetPIDs(files[i]);
		
		for (size_t k = 0; k < processes.size(); k++)
        {
            const ProcessInfo& process = processes[k]; 
			CString sProcessName = CString(files[i]) + _T(" (") + CString(process.fullPath.c_str()) + _T(")");
            CString sStatus = m_CheckSuspend.IsProcessSuspended(process.pid) ? _T("일시정지") : _T("실행 중");            
			InsertList(sProcessName, sStatus, process.pid); 
        }
	}

	//미실행 프로세스 제거
    for (int i = 0; i < m_ListCtrl.GetItemCount(); )
    {
        DWORD processID = m_ProcessIDs[i];
        if (m_CheckSuspend.IsProcessRunning(processID))
        {
            ++i;
        }
        else
        {
            m_ListCtrl.DeleteItem(i);
            m_ProcessIDs.erase(m_ProcessIDs.begin() + i);
        }
    }
}

void CFindSuspendDlg::ToggleProcessState(int index)
{
    if (index < 0 || index >= static_cast<int>(m_ProcessIDs.size()))
        return;
    
    DWORD processID = m_ProcessIDs[index];
    bool isSuspended = m_CheckSuspend.IsProcessSuspended(processID);
    if (isSuspended)
    {
        m_CheckSuspend.PauseResumeThreadList(processID, true);
        m_ListCtrl.SetItemText(index, 1, _T("실행 중"));
    }
    else
    {
        m_CheckSuspend.PauseResumeThreadList(processID, false);
        m_ListCtrl.SetItemText(index, 1, _T("일시정지"));
    }
}

void CFindSuspendDlg::UpdateProcessState(int index)
{
    if (index < 0 || index >= static_cast<int>(m_ProcessIDs.size()))
        return;
    
    DWORD processID = m_ProcessIDs[index];
    bool isSuspended = m_CheckSuspend.IsProcessSuspended(processID);
    CString status = isSuspended ? _T("일시정지") : _T("실행 중");
    m_ListCtrl.SetItemText(index, 1, status);
}