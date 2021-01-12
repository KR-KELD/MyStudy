#pragma once
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <tchar.h>
#include <string>
#include <vector>
#pragma comment(lib,"odbc32.lib")

///
//ODBC 데이터 원본(64비트) 드라이버 설치
//설치프로그램->Microsoft Access Database Engine 2010 재배포 가능 패키지
//https ://www.microsoft.com/ko-kr/download/details.aspx?id=13255
///

class myQuery
{
public:
	// 1)핸들선언
	SQLHENV  m_hEnv = SQL_NULL_HENV; // 환경핸들
	SQLHENV  m_hDbc = SQL_NULL_HDBC; // 연결핸들
	SQLHENV  m_hStmt = SQL_NULL_HSTMT;// 명령핸들
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

