#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h> // windows.h 보다 우선 연결한다.
#include <stdio.h>
#include <tchar.h>
#include <time.h>
//#include <Ws2tcpip.h>	//inet_pton 헤더
#include <process.h>
#include "myProtocol.h"
#include "myUtil.h"
#pragma comment (lib, "ws2_32.lib")

static void Error(const TCHAR* szHeader = L"ERROR", const TCHAR* szMsg = L"\0")
{
	LPVOID* lpMsg = 0;;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(TCHAR*)lpMsg, 0, NULL);
	std::wstring szBuffer = szMsg;
	szBuffer += L"\n";
	szBuffer += (TCHAR*)lpMsg;
	MessageBox(NULL, szBuffer.c_str(), szHeader, MB_ICONERROR);
	LocalFree(lpMsg);
}

static bool Check(int iRet, int line)
{
	if (iRet == SOCKET_ERROR)
	{
		TCHAR szBuffer[256] = { 0, };
		_stprintf_s(szBuffer, _countof(szBuffer),
			L"%s\n[line:%d]", __FILE__, line);
		Error(L"ERROR", szBuffer);
		exit(1);
	}
}
