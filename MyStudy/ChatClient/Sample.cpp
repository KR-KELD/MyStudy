#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <stdio.h>
#include <tchar.h>
#include <time.h>
#include <conio.h>
#include <Ws2tcpip.h>	//inet_pton 헤더
#pragma comment (lib, "ws2_32.lib")

struct myMsg
{
	int  iCount;
	char buffer[3000];
};

void main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}
	int iRet;
	//TCP-IP
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//LINGER
	int optval = 1;
	linger optLinger;
	int iLingerLen = sizeof(optLinger);
	optLinger.l_onoff = 1;
	optLinger.l_linger = 1000;
	if (setsockopt(sock, SOL_SOCKET, SO_LINGER,
		(char*)&optLinger, iLingerLen) != 0)
	{
		return;
	}
	SOCKADDR_IN sa;
	// 바이트 정렬 구조 	
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr("175.194.89.106");
	//error C4996 : 'inet_addr' : Use inet_pton() or InetPton() instead or define _WINSOCK_DEPRECATED_NO_WARNINGS
	sa.sin_port = htons(10000);
	iRet = connect(sock, (SOCKADDR*)&sa, sizeof(sa));
	if (iRet == SOCKET_ERROR)
	{
		return;
	}
	//넌블로킹 소켓으로 전환
	unsigned long iMode = 1;
	ioctlsocket(sock, FIONBIO, &iMode);

	myMsg msg;
	int iRecvSize = 0;
	int iSendSize = 0;
	int iPacketSize = sizeof(myMsg);
	char recvBuf[10000] = { 0, };
	char sendBuf[10000] = { 0, };
	int iMsgLength = 0;
	int iCount = 0;
	bool bConnect = true;
	while (bConnect)
	{
		//키보드 입력 받기
		if (_kbhit() != 0)
		{
			//키 입력을 아스키코드로 저장
			int iValue = _getche();
			//샌드버퍼의 길이
			int ilen = strlen(sendBuf);
			if (ilen == 0 && iValue == '\r')
			{
				memset(sendBuf, 0, sizeof(char) * 10000);
				continue;
			}
			//엔터를 치면 전송
			if (iValue == '\r')
			{
				//샌드 버퍼의 내용을 메시지 구조체에 담아서 서버에 보내기
				memset(&msg, 0, sizeof(msg));
				strcpy_s(msg.buffer, sendBuf);
				msg.iCount = iCount++;

				iMsgLength = 0;
				//데이터 하나를 온전히 보내라
				while (iSendSize < iPacketSize)
				{
					iSendSize += send(sock, (char*)&msg,
						iPacketSize - iSendSize, 0);
					//에러이거나 샌드메시지가 비어있을경우
					if (iSendSize == 0 || iSendSize == SOCKET_ERROR)
					{
						//에러일경우
						if (WSAGetLastError() != WSAEWOULDBLOCK)
						{
							bConnect = false;
						}
						break;
					}
				}
				//데이터를 보내고 샌드버퍼를 초기화
				memset(sendBuf, 0, sizeof(char) * 10000);
			}
			else
			{
				//엔터가 아닐경우는 샌드버퍼를 채운다
				sendBuf[iMsgLength++] = iValue;
			}
		}
		//리시브 버퍼 초기화
		memset(recvBuf, 0, sizeof(char) * 10000);

		//데이터 하나를 온전히 받아라
		while (iRecvSize < iPacketSize)
		{
			iRecvSize += recv(sock, recvBuf,
				iPacketSize - iRecvSize, 0);

			if (iRecvSize == 0 || iRecvSize == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSAEWOULDBLOCK)
				{
					bConnect = false;
				}
				break;
			}
			if (sizeof(myMsg) == iRecvSize)
			{
				memcpy(&msg, recvBuf, sizeof(myMsg));
				printf("\nServer:[%d]:%s\n",
					msg.iCount, msg.buffer);
			}
		}
		iSendSize = 0;
		iRecvSize = 0;
	}
	shutdown(sock, SD_SEND);
	closesocket(sock);
	WSACleanup();
}