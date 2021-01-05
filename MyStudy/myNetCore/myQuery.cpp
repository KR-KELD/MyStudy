#include "myQuery.h"

bool myQuery::Check(SQLRETURN ret)
{
	if (ret == SQL_SUCCESS) return false;
	SQLTCHAR szSQLState[SQL_SQLSTATE_SIZE + 1];
	SQLTCHAR errorBuffer[SQL_MAX_MESSAGE_LENGTH + 1];
	SQLINTEGER iSQLCode;
	SQLSMALLINT length;
	SQLError(m_hEnv, m_hDbc,
		m_hStmt,
		szSQLState,
		&iSQLCode,
		errorBuffer,
		sizeof(errorBuffer),
		&length);
	MessageBox(NULL, errorBuffer, szSQLState, MB_OK);
	return true;
}

bool myQuery::Connect(std::wstring strFileName)
{
	SQLRETURN ret;
	TCHAR  InCon[256] = { 0, };
	TCHAR  Dir[MAX_PATH];
	SQLSMALLINT cbCon;
	GetCurrentDirectory(MAX_PATH, Dir);
	wsprintf(InCon,
		_T("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};DBQ=%s\\%s;"), Dir, strFileName.c_str());
	if(Check(SQLDriverConnect(m_hDbc, NULL, InCon, _countof(InCon), NULL, 0, &cbCon, SQL_DRIVER_NOPROMPT)))
	{
		return false;
	}
	if (Check(SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hStmt)))
	{
		return false;
	}
	return true;
}

bool myQuery::Exec(const TCHAR * sql)
{
	SQLRETURN ret;
	SQLTCHAR  userid[30] = { 0, };
	SQLTCHAR  userps[30] = { 0, };
	SQLINTEGER id;
	SQLLEN  lID, lUserID, lUserPS;
	SQLBindCol(m_hStmt, 1, SQL_INTEGER, &id, 0, &lID);
	SQLBindCol(m_hStmt, 2, SQL_UNICODE, (SQLPOINTER)userid, sizeof(userid), &lUserID);
	SQLBindCol(m_hStmt, 3, SQL_UNICODE, (SQLPOINTER)userps, sizeof(userps), &lUserPS);
	//TCHAR sql[] = L"select * from DemoGame";
	ret = SQLExecDirect(m_hStmt, (SQLTCHAR*)sql, SQL_NTS);
	SQLLEN iNumRow;
	SQLSMALLINT iNumCol;
	SQLRowCount(m_hStmt, &iNumRow); // sql 영향을 받는 레코드 수
	SQLNumResultCols(m_hStmt, &iNumCol); // 반환 컬럼 수

	while (SQLFetch(m_hStmt) != SQL_NO_DATA)
	{
		//std::wcout << "id=" << id << " " << userid << " " << userps << std::endl;
	}
	SQLCloseCursor(m_hStmt);
	return true;
}

bool myQuery::ExecSelect(const TCHAR * sql, std::wstring & szID, std::wstring & szPS)
{
	return true;
}

bool myQuery::ExecUpdate(const TCHAR * sql)
{
	return true;
}

bool myQuery::ExecDelete(const TCHAR * sql)
{
	return true;
}

bool myQuery::ExecInsert(const TCHAR * sql)
{
	return true;
}

bool myQuery::Init()
{
	// 2)핸들 할당 -> 설정
	if (Check(SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv)))
	{
		return false;
	}
	if (Check(SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0)))
	{
		return false;
	}
	if (Check(SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, &m_hDbc)))
	{
		return false;
	}
	return true;
}

bool myQuery::Release()
{
	SQLFreeHandle(SQL_HANDLE_STMT, m_hStmt);
	SQLFreeHandle(SQL_HANDLE_DBC, m_hDbc);
	SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
	return true;
}

myQuery::myQuery()
{
}

myQuery::~myQuery()
{
}
