#include <iostream>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <tchar.h>
#pragma comment(lib,"odbc32.lib")
void main()
{
	//한국어 전환
	setlocale(LC_ALL, "KOREAN");
	// 1)핸들선언
	SQLHENV  hEnv = SQL_NULL_HENV; // 환경핸들
	SQLHENV  hDbc = SQL_NULL_HDBC; // 연결핸들
	SQLHENV  hStmt = SQL_NULL_HSTMT;// 명령핸들
	// 2)핸들 할당 -> 설정
	SQLRETURN ret;
	//환경 핸들 초기화
	ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
	//환경 핸들 세팅
	ret = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
	//연결 핸들 초기화
	ret = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);

	TCHAR  InCon[256] = { 0, };
	TCHAR  Dir[MAX_PATH];
	SQLSMALLINT cbCon;
	//현재 폴더 주소 가져오기
	GetCurrentDirectory(MAX_PATH, Dir);
	//연결 명령어
	wsprintf(InCon, _T("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};DBQ=%s\\%s;"), Dir, L"DemoGame.accdb");
	//연결
	ret = SQLDriverConnect(hDbc, NULL, InCon, _countof(InCon), NULL, 0,
		&cbCon, SQL_DRIVER_NOPROMPT);
	//명령 핸들 초기화
	ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
	if (ret != SQL_SUCCESS)
	{
		return;
	}
	// 3)핸들 사용 ->조회
	{
		//받을 데이터형을 선언
		SQLTCHAR  userid[30] = { 0, };
		SQLTCHAR  userps[30] = { 0, };
		SQLINTEGER id;
		SQLLEN  lID, lUserID, lUserPS;
		SQLBindCol(hStmt, 1, SQL_INTEGER, &id, 0, &lID);
		SQLBindCol(hStmt, 2, SQL_UNICODE, (SQLPOINTER)userid, sizeof(userid), &lUserID);
		SQLBindCol(hStmt, 3, SQL_UNICODE, (SQLPOINTER)userps, sizeof(userps), &lUserPS);
		//DemoGame1 테이블을 선택해서 그 안의 모든 데이터를 조회한다
		TCHAR sql[] = L"select * from DemoGame1";
		//직접 명령
		ret = SQLExecDirect(hStmt, (SQLTCHAR*)sql, SQL_NTS);
		SQLLEN iNumRow;
		SQLSMALLINT iNumCol;
		SQLRowCount(hStmt, &iNumRow); // sql 영향을 받는 레코드 수
		SQLNumResultCols(hStmt, &iNumCol); // 반환 컬럼 수

		while (SQLFetch(hStmt) != SQL_NO_DATA)
		{
			std::wcout << "id=" << id << " " << userid << " " << userps << std::endl;
		}
		SQLCloseCursor(hStmt);
	}
	// 3)핸들 사용 ->추가
	{
		//DemoGame1 테이블에 userID(필드)가 Game이고 userPW(필드)가 4444인 레코드를 추가해라
		TCHAR sql[] =
			L"insert into DemoGame1 (userID, userPW) VALUES ('Game','4444')";
		if (SQLExecDirect(hStmt, (SQLTCHAR*)sql, SQL_NTS) != SQL_SUCCESS)
		{
			return;
		}
		SQLLEN iNumRow;
		SQLSMALLINT iNumCol;
		SQLRowCount(hStmt, &iNumRow); // sql 영향을 받는 레코드 수
		SQLNumResultCols(hStmt, &iNumCol); // 반환 컬럼 수
		SQLCloseCursor(hStmt);
	}
	//// 3)핸들 사용 ->삭제
	{
		//DemoGame1 테이블에 userID(필드)가 Game인 레코드를 제거해라
		TCHAR sql[] =
			L"delete from DemoGame1 where userID='Game'";
		if (SQLExecDirect(hStmt, (SQLTCHAR*)sql, SQL_NTS) != SQL_SUCCESS)
		{
			//return;
		}
		SQLLEN iNumRow;
		SQLSMALLINT iNumCol;
		SQLRowCount(hStmt, &iNumRow); // sql 영향을 받는 레코드 수
		SQLNumResultCols(hStmt, &iNumCol); // 반환 컬럼 수
		SQLCloseCursor(hStmt);
	}
	// 수정
	{
		//DemoGame1 테이블에 userID(필드)가 kim인 레코드의 userPW(필드)를 3333으로 수정해라
		TCHAR sql[] =
			L"update DemoGame1 set userPW='3333' where userID='kim'";
		if (SQLExecDirect(hStmt, (SQLTCHAR*)sql, SQL_NTS) != SQL_SUCCESS)
		{
			return;
		}
		SQLLEN iNumRow;
		SQLSMALLINT iNumCol;
		SQLRowCount(hStmt, &iNumRow); // sql 영향을 받는 레코드 수
		SQLNumResultCols(hStmt, &iNumCol); // 반환 컬럼 수
		SQLCloseCursor(hStmt);
	}
	{
		SQLTCHAR  userid[30] = { 0, };
		SQLTCHAR  userps[30] = { 0, };
		SQLINTEGER id;
		SQLLEN  lID, lUserID, lUserPS;
		SQLBindCol(hStmt, 1, SQL_INTEGER, &id, 0, &lID);
		SQLBindCol(hStmt, 2, SQL_UNICODE, (SQLPOINTER)userid, sizeof(userid), &lUserID);
		SQLBindCol(hStmt, 3, SQL_UNICODE, (SQLPOINTER)userps, sizeof(userps), &lUserPS);
		TCHAR sql[] = L"select * from DemoGame1";
		ret = SQLExecDirect(hStmt, (SQLTCHAR*)sql, SQL_NTS);
		while (SQLFetch(hStmt) != SQL_NO_DATA)
		{
			std::wcout << "id=" << id << " " << userid << " " << userps << std::endl;
		}
		SQLCloseCursor(hStmt);
	}
	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
	SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

}