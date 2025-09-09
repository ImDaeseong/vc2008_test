#include "StdAfx.h"
#include "DBInfo.h"

/*
//추가포함 디렉토리
C:\Program Files (x86)\MySQL\MySQL Connector C 6.1\include

//추가 라이브러리 디렉토리
C:\Program Files (x86)\MySQL\MySQL Connector C 6.1\lib

//추가종속성
libmysql.lib
*/

CDBInfo::CDBInfo()
{
	m_pConnection = NULL;
    m_pResult = NULL;
}

CDBInfo::~CDBInfo()
{
	Disconnect();
}

//접속
BOOL CDBInfo::Connect(const CString& IP, const CString& ID, const CString& PWD, const CString& DB, unsigned int PORT)
{
    m_pConnection = mysql_init(NULL);

    if (m_pConnection == NULL)
	{
        return FALSE;
    }

    if (mysql_real_connect(m_pConnection, CT2A(IP), CT2A(ID), CT2A(PWD), CT2A(DB), PORT, NULL, 0) == NULL)
    {
        mysql_close(m_pConnection);
        m_pConnection = NULL;
        return FALSE;
    }

    mysql_set_character_set(m_pConnection, "utf8");

    return TRUE;
}

//접속 해제
void CDBInfo::Disconnect()
{
    if (m_pResult) 
	{
        mysql_free_result(m_pResult);
        m_pResult = NULL;
    }

    if (m_pConnection) 
	{
        mysql_close(m_pConnection);
        m_pConnection = NULL;
    }
}

//쿼리
BOOL CDBInfo::ExecuteQuery(const CString& query)
{
    if (m_pConnection == NULL)
	{
        return FALSE;
    }

    if (m_pResult)
	{
        mysql_free_result(m_pResult);
        m_pResult = NULL;
    }

    if (mysql_query(m_pConnection, CT2A(query)) != 0) 
	{
        return FALSE;
    }

    m_pResult = mysql_store_result(m_pConnection);
    if (m_pResult == NULL && mysql_field_count(m_pConnection) > 0) 
	{
        return FALSE;
    }

    return TRUE;
}

//데이터 조회
vector<MYInfo> CDBInfo::GetMyInfoList()
{
    vector<MYInfo> info;

    if (!ExecuteQuery(_T("SELECT first, second FROM my_info"))) 
	{
        return info;
    }

    if (m_pResult == NULL) 
	{
        return info;
    }

    while ((m_row = mysql_fetch_row(m_pResult)) != NULL) 
	{
        CString first = m_row[0] ? CA2T(m_row[0]) : _T("");
        CString second   = m_row[1] ? CA2T(m_row[1]) : _T("");
        info.push_back(MYInfo(first, second));
    }

    return info;
}

//조회 데이터 읽기
void CDBInfo::ReadList(const vector<MYInfo>& info)
{
	CString index, first, second;

	for (size_t i = 0; i < info.size(); ++i)
    {
        index.Format(_T("%u"), (UINT)(i + 1));
        first.Format(_T("%s"), info[i].First);
        second.Format(_T("%s"), info[i].Second);
    }
}
