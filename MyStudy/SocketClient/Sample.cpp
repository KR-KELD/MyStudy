#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <stdio.h>
#include <tchar.h>
#include <time.h>
#include <string>

#pragma comment (lib, "ws2_32.lib")

struct myMsg
{
	int  iCount;
	char buffer[3000];
};

void Error(const TCHAR* szHeader, const TCHAR* szMsg)
{
	TCHAR* lpMsg = 0;;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
				  FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
		lpMsg,0,NULL);

	MessageBox(NULL, lpMsg, szHeader, MB_ICONERROR);
	LocalFree(lpMsg);
}

void Check(int iRet, int line)
{
	if (iRet == SOCKET_ERROR)
	{
		TCHAR szBuffer[256] = { 0, };
		_stprintf_s(szBuffer, _countof(szBuffer),
			L"%s\n[line:%d]\n", __FILE__, line);
		Error(L"ERROR", szBuffer);
		exit(1);
	}
}
void main()
{
	// 2.2 ver
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}
	while (1)
	{
		int iRet;
		// �ּ�ü��
		// ����Ÿ��(TCP:SOCK_STREAM,UDP:SOCK_DGRAM)
		// ��������(IPPROTO_TCP,    IPPROTO_UDP )
		SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sock == INVALID_SOCKET)
		{
			//Check(L"Socket");
			return;
		}
		int optval = 1;
		if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,
			(char*)&optval, sizeof(optval)) != 0)
		{
			break;
		}
		/*linger optLinger;
		int iLingerLen = sizeof(optLinger);
		optLinger.l_onoff = 1;
		optLinger.l_linger = 1000;
		if (setsockopt(sock, SOL_SOCKET, SO_LINGER,
			(char*)&optLinger, iLingerLen) != 0)
		{
			break;
		}*/

		SOCKADDR_IN sa;
		// ����Ʈ ���� ���� 	
		USHORT jValue = 10000;
		sa.sin_family = AF_INET;

		sa.sin_addr.s_addr = inet_addr("");
		//sa.sin_addr.s_addr = inet_addr("175.194.89.106");
		//error C4996 : 'inet_addr' : Use inet_pton() or InetPton() instead or define _WINSOCK_DEPRECATED_NO_WARNINGS
		sa.sin_port = htons(10000);
		iRet = connect(sock, (SOCKADDR*)&sa, sizeof(sa));
		if (iRet == SOCKET_ERROR)
		{
			return;
		}
		int iCount = 0;
		clock_t s = clock();
		clock_t e = clock();
		myMsg msg;

		int iRecvSize = 0;
		int iSendSize = 0;
		int iPacketSize = sizeof(myMsg);
		char recvBuf[10000] = { 0, };

		bool bConnect = true;
		while (e - s < 1000 && bConnect)
		{
			while (iSendSize < iPacketSize)
			{
				memset(&msg, 0, sizeof(msg));
				strcpy_s(msg.buffer, 32, "Ŭ���̾�Ʈ�޽���");
				msg.iCount = iCount;
				char recvBuf[3001] = { 0, };
				clock_t t1 = clock();
				iSendSize += send(sock, (char*)&msg,
					iPacketSize - iSendSize, 0);
				if (iSendSize == 0 || iSendSize == SOCKET_ERROR)
				{
					bConnect = false;
					break;
				}
			}
			memset(recvBuf, 0, sizeof(char) * 10000);
			while (iRecvSize < iPacketSize && bConnect)
			{
				iRecvSize += recv(sock, recvBuf,
					iPacketSize - iRecvSize, 0);
				if (iRecvSize == 0 || iRecvSize == SOCKET_ERROR)
				{
					bConnect = false;
					break;
				}
				if (sizeof(myMsg) == iRecvSize)
				{
					memcpy(&msg, recvBuf, sizeof(myMsg));
					printf("\n%d:%s",
						msg.iCount, msg.buffer);

				}
			}
			iSendSize = 0;
			iRecvSize = 0;
			e = clock();
			clock_t t = e - s;
			//printf("%d ", t);
			iCount++;
		}
		//shutdown(sock, SD_SEND);
		closesocket(sock);
		Sleep(1000);
	}
	WSACleanup();
}