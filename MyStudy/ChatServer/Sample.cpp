#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <stdio.h>
#include <tchar.h>
#include <time.h>
#include <string>
#include <vector>
#include <list>
#include <Ws2tcpip.h>	//inet_pton 헤더
#pragma comment (lib, "ws2_32.lib")
using namespace std;

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
	vector<myMsg> recvlist;
	int iRet;
	//TCP-IP
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
	{
		return;
	}
	//소켓 타입 확인
	int iSockType;
	//데이터 타입 길이
	int iSockTypeLen = sizeof(int);
	//소켓 옵션 조회-소켓 타입
	getsockopt(sock, SOL_SOCKET, SO_TYPE,
		(char*)&iSockType, &iSockTypeLen);
	//TCP인지 UDP인지
	if (iSockType == SOCK_STREAM)
		printf("%s\r\n", "SOCK_STREAM.");
	else
		printf("%s\r\n", "SOCK_DGRAM");

	//송수신 버퍼 크기 확인	
	int iSendBuffer = 100000;
	//소켓 옵션 설정-샌드버퍼 크기
	setsockopt(sock, SOL_SOCKET, SO_SNDBUF,
		(char*)&iSendBuffer, sizeof(iSendBuffer));
	//소켓 옵션 조회-샌드버퍼 크기
	getsockopt(sock, SOL_SOCKET, SO_SNDBUF,
		(char*)&iSockType, &iSockTypeLen);
	printf("SendBuffer=%d\n", iSockType);
	//소켓 옵션 조회-리시브버퍼 크기
	getsockopt(sock, SOL_SOCKET, SO_RCVBUF,
		(char*)&iSockType, &iSockTypeLen);
	printf("RecvBuffer=%d\n", iSockType);
	int iOptValue = 1;
	iRet = setsockopt(sock, SOL_SOCKET, SO_EXCLUSIVEADDRUSE,
		(char*)&iOptValue, sizeof(iOptValue));
	if (iRet == SOCKET_ERROR)
	{
		return;
	}

	//클라이언트의 연결이 끊겼을 때
	//대기시간을 주는 옵션
	linger optLinger;
	int iLingerLen = sizeof(optLinger);
	//설정
	optLinger.l_onoff = 1;
	//시간
	optLinger.l_linger = 1000;
	if (setsockopt(sock, SOL_SOCKET, SO_LINGER,
		(char*)&optLinger, iLingerLen) != 0)
	{
		return;
	}
	//주소
	//바이트 정렬 구조 	
	SOCKADDR_IN sa;
	//포트 번호
	USHORT jValue = 10000;
	sa.sin_family = AF_INET;
	//ip주소
	//모든 어드레스를 받겠다
	sa.sin_addr.s_addr = inet_addr(INADDR_ANY);//inet_addr("175.194.89.106");
	//error C4996 : 'inet_addr' : Use inet_pton() or InetPton() instead or define _WINSOCK_DEPRECATED_NO_WARNINGS
	//htons -> 호스트의 메모리 체계에서 네트워크의 메모리 체계로 변환 해 주는 함수
	sa.sin_port = htons(10000);

	//SO_REUSEADDR가 설정된 소켓이 있을 경우는 오류가 된다.
	iRet = bind(sock, (SOCKADDR*)&sa, sizeof(sa));
	if (iRet == SOCKET_ERROR)
	{
		return;
	}
	//서버가 요청을 받을 수 있는 상태로 만들어 준다.
	iRet = listen(sock, SOMAXCONN);

	//클라이언트 주소
	SOCKADDR_IN clientAddr;
	int len = sizeof(clientAddr);

	// 넌블로킹 소켓으로 전환
	unsigned long iMode = 1;
	ioctlsocket(sock, FIONBIO, &iMode);

	list<SOCKET> userList;
	list<SOCKET>::iterator userIter;
	while (true)
	{
		//연결 확인
		SOCKET client = accept(sock, (SOCKADDR*)&clientAddr, &len);
		if (client == INVALID_SOCKET)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				break;
			}
		}
		else
		{
			userList.push_back(client);
			printf("\n접속->%s:%d", inet_ntoa(clientAddr.sin_addr),
				ntohs(clientAddr.sin_port));
		}
		myMsg msg;
		//받는 버퍼
		char recvBuf[10000] = { 0, };
		//받은 버퍼 크기
		int iRecvSize = 0;
		//보낼 버퍼 크기
		int iSendSize = 0;
		int iPacketSize = sizeof(myMsg);

		for (userIter = userList.begin();
			userIter != userList.end();)
		{
			while (iRecvSize < iPacketSize)
			{
				iRecvSize += recv(*userIter, recvBuf,
					iPacketSize - iRecvSize, 0);
				if (iRecvSize == 0 || iRecvSize == SOCKET_ERROR)
				{
					if (WSAGetLastError() != WSAEWOULDBLOCK)
					{

					}
				}
			}
		}

	}
}