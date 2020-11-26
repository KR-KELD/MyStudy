#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <stdio.h>
#include <tchar.h>
#include <time.h>
#pragma comment (lib, "ws2_32.lib")

struct myMsg
{
	int  iCount;
	char buffer[3000];
};
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
		// 주소체계
		// 소켓타입(TCP:SOCK_STREAM,UDP:SOCK_DGRAM)
		// 프로토콜(IPPROTO_TCP,    IPPROTO_UDP )
		SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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
		// 바이트 정렬 구조 	
		USHORT jValue = 10000;
		sa.sin_family = AF_INET;
		sa.sin_addr.s_addr = inet_addr("175.194.89.106");
		//error C4996 : 'inet_addr' : Use inet_pton() or InetPton() instead or define _WINSOCK_DEPRECATED_NO_WARNINGS
		sa.sin_port = htons(10000);
		int iRet = connect(sock, (SOCKADDR*)&sa, sizeof(sa));
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
				strcpy_s(msg.buffer, 32, "클라이언트메시지");
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
