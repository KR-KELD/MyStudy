#pragma once
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <tchar.h>
#include <string>
#include <vector>
#pragma comment(lib,"odbc32.lib")

///
//ODBC ������ ����(64��Ʈ) ����̹� ��ġ
//��ġ���α׷�->Microsoft Access Database Engine 2010 ����� ���� ��Ű��
//https ://www.microsoft.com/ko-kr/download/details.aspx?id=13255
///

class myQuery
{
public:
	// 1)�ڵ鼱��
	SQLHENV  m_hEnv = SQL_NULL_HENV; // ȯ���ڵ�
	SQLHENV  m_hDbc = SQL_NULL_HDBC; // �����ڵ�
	SQLHENV  m_hStmt = SQL_NULL_HSTMT;// ����ڵ�
public:
	bool	Check(SQLRETURN ret);
	bool	Connect(std::wstring strFileName);
	bool	Exec(const TCHAR* sql);
	bool	ExecSelect(const TCHAR * sql, std::wstring& szID, std::wstring& szPS);
	bool	ExecUpdate(const TCHAR * sql);
	bool	ExecDelete(const TCHAR * sql);
	bool	ExecInsert(const TCHAR * sql);
public:
	virtual bool Init();
	virtual bool Release();
public:
	myQuery();
	virtual ~myQuery();
};

