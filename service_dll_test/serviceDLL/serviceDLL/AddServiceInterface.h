#ifndef	__ADDSERVICEINTERFACE_H__
#define	__ADDSERVICEINTERFACE_H__

extern "C"
{

	__declspec(dllimport) BOOL	ApiInstallService(LPSTR szFilename);

	__declspec(dllimport) BOOL ApiUninstallService();

	__declspec(dllimport) BOOL ApiStartService();

	__declspec(dllimport) BOOL ApiStopService();

};

#endif