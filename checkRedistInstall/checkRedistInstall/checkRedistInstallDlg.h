#pragma once

class CcheckRedistInstallDlg : public CDialog
{
public:
	CcheckRedistInstallDlg(CWnd* pParent = NULL);	// standard constructor

	enum { IDD = IDD_CHECKREDISTINSTALL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

private:
	BOOL isInstallDotNetframework();
	BOOL InstallDotNetframework();

	BOOL isRedistPackage(bool isX64);
	BOOL InstallRedistPackage(bool isX64);

	BOOL isEdgeBrowswer();
	BOOL InstallEdgeBrowswer(bool isX64);

	BOOL isMariaDB();
	BOOL InstallMariaDB();
	BOOL CreateServiceMariaDB();
	BOOL StartServiceMariaDB();
	BOOL CreateRootPasswordMariaDB();
	BOOL ConfigureMariaDBFirewall();
};
