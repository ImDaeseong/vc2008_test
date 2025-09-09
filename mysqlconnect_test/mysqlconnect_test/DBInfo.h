#pragma once

#include "mysql.h"
#pragma comment(lib, "libmysql.lib")

#include <vector>
using namespace std;


struct MYInfo
{
    CString First;
    CString Second;

    MYInfo() {}
    MYInfo(const CString& first, const CString& second)
        : First(first), Second(second) {}
};


class CDBInfo
{
public:
	CDBInfo();
	~CDBInfo();

	BOOL Connect(const CString& IP, const CString& ID, const CString& PWD, const CString& DB, unsigned int PORT);

    void Disconnect();

    BOOL ExecuteQuery(const CString& query);

    vector<MYInfo> GetMyInfoList();
    
	void ReadList(const vector<MYInfo>& info);

private:
    MYSQL* m_pConnection;
    MYSQL_RES* m_pResult;
    MYSQL_ROW m_row;
};
