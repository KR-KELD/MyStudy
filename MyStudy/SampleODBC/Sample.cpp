#include <iostream>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <tchar.h>
#pragma comment(lib,"odbc32.lib")
void main()
{
	//�ѱ��� ��ȯ
	setlocale(LC_ALL, "KOREAN");
	// 1)�ڵ鼱��
	SQLHENV  hEnv = SQL_NULL_HENV; // ȯ���ڵ�
	SQLHENV  hDbc = SQL_NULL_HDBC; // �����ڵ�
	SQLHENV  hStmt = SQL_NULL_HSTMT;// ����ڵ�
	// 2)�ڵ� �Ҵ� -> ����
	SQLRETURN ret;
	//ȯ�� �ڵ� �ʱ�ȭ
	ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
	//ȯ�� �ڵ� ����
	ret = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
	//���� �ڵ� �ʱ�ȭ
	ret = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);

	TCHAR  InCon[256] = { 0, };
	TCHAR  Dir[MAX_PATH];
	SQLSMALLINT cbCon;
	//���� ���� �ּ� ��������
	GetCurrentDirectory(MAX_PATH, Dir);
	//���� ��ɾ�
	wsprintf(InCon, _T("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};DBQ=%s\\%s;"), Dir, L"DemoGame.accdb");
	//����
	ret = SQLDriverConnect(hDbc, NULL, InCon, _countof(InCon), NULL, 0,
		&cbCon, SQL_DRIVER_NOPROMPT);
	//��� �ڵ� �ʱ�ȭ
	ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
	if (ret != SQL_SUCCESS)
	{
		return;
	}
	// 3)�ڵ� ��� ->��ȸ
	{
		//���� ���������� ����
		SQLTCHAR  userid[30] = { 0, };
		SQLTCHAR  userps[30] = { 0, };
		SQLINTEGER id;
		SQLLEN  lID, lUserID, lUserPS;
		SQLBindCol(hStmt, 1, SQL_INTEGER, &id, 0, &lID);
		SQLBindCol(hStmt, 2, SQL_UNICODE, (SQLPOINTER)userid, sizeof(userid), &lUserID);
		SQLBindCol(hStmt, 3, SQL_UNICODE, (SQLPOINTER)userps, sizeof(userps), &lUserPS);
		//DemoGame1 ���̺��� �����ؼ� �� ���� ��� �����͸� ��ȸ�Ѵ�
		TCHAR sql[] = L"select * from DemoGame1";
		//���� ���
		ret = SQLExecDirect(hStmt, (SQLTCHAR*)sql, SQL_NTS);
		SQLLEN iNumRow;
		SQLSMALLINT iNumCol;
		SQLRowCount(hStmt, &iNumRow); // sql ������ �޴� ���ڵ� ��
		SQLNumResultCols(hStmt, &iNumCol); // ��ȯ �÷� ��

		while (SQLFetch(hStmt) != SQL_NO_DATA)
		{
			std::wcout << "id=" << id << " " << userid << " " << userps << std::endl;
		}
		SQLCloseCursor(hStmt);
	}
	// 3)�ڵ� ��� ->�߰�
	{
		//DemoGame1 ���̺� userID(�ʵ�)�� Game�̰� userPW(�ʵ�)�� 4444�� ���ڵ带 �߰��ض�
		TCHAR sql[] =
			L"insert into DemoGame1 (userID, userPW) VALUES ('Game','4444')";
		if (SQLExecDirect(hStmt, (SQLTCHAR*)sql, SQL_NTS) != SQL_SUCCESS)
		{
			return;
		}
		SQLLEN iNumRow;
		SQLSMALLINT iNumCol;
		SQLRowCount(hStmt, &iNumRow); // sql ������ �޴� ���ڵ� ��
		SQLNumResultCols(hStmt, &iNumCol); // ��ȯ �÷� ��
		SQLCloseCursor(hStmt);
	}
	//// 3)�ڵ� ��� ->����
	{
		//DemoGame1 ���̺� userID(�ʵ�)�� Game�� ���ڵ带 �����ض�
		TCHAR sql[] =
			L"delete from DemoGame1 where userID='Game'";
		if (SQLExecDirect(hStmt, (SQLTCHAR*)sql, SQL_NTS) != SQL_SUCCESS)
		{
			//return;
		}
		SQLLEN iNumRow;
		SQLSMALLINT iNumCol;
		SQLRowCount(hStmt, &iNumRow); // sql ������ �޴� ���ڵ� ��
		SQLNumResultCols(hStmt, &iNumCol); // ��ȯ �÷� ��
		SQLCloseCursor(hStmt);
	}
	// ����
	{
		//DemoGame1 ���̺� userID(�ʵ�)�� kim�� ���ڵ��� userPW(�ʵ�)�� 3333���� �����ض�
		TCHAR sql[] =
			L"update DemoGame1 set userPW='3333' where userID='kim'";
		if (SQLExecDirect(hStmt, (SQLTCHAR*)sql, SQL_NTS) != SQL_SUCCESS)
		{
			return;
		}
		SQLLEN iNumRow;
		SQLSMALLINT iNumCol;
		SQLRowCount(hStmt, &iNumRow); // sql ������ �޴� ���ڵ� ��
		SQLNumResultCols(hStmt, &iNumCol); // ��ȯ �÷� ��
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