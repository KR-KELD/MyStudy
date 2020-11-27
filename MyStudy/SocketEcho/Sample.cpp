#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <stdio.h>
#include <tchar.h>
#include <time.h>
#include <string>
#include <vector>
using namespace std;
#pragma comment (lib, "ws2_32.lib")

void Error(const TCHAR* msg)
{
	LPVOID lpMsg = 0;;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(TCHAR*)&lpMsg, 0, NULL);
	MessageBox(NULL, (TCHAR*)lpMsg, msg, MB_ICONERROR);
	LocalFree(lpMsg);
}

bool Check(int iRet)
{
	return false;
}

struct myMsg
{
	int  iCount;
	char buffer[3000];
};
//서버
void main()
{
	// 2.2 ver
	WSADATA wsa;
	//WSA2.2버전 연결
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}
	//받은 메시지 리스트
	vector<myMsg> recvlist;

	int iRet;
	//소켓 생성 TCP타입
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
	{
		//Check(L"Socket");
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

	//소켓 옵션 설정
	//프로세스에서 1개의 ip와 port만 연결 가능하다
	//bind 함수에서 결과 리턴
	iRet = setsockopt(sock, SOL_SOCKET, SO_EXCLUSIVEADDRUSE,
		(char*)&iOptValue, sizeof(iOptValue));
	if (iRet == SOCKET_ERROR)
	{
		return;
	}
	//여러 프로세스에서 연결이 가능하다
	/*iRet = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,
		(char*)&optval, sizeof(optval));
	if (iRet == SOCKET_ERROR)
	{
		return;
	}*/

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
	sa.sin_addr.s_addr = inet_addr("192.168.0.169");
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
	bool bConnect = false;
	//클라이언트 주소
	SOCKADDR_IN clientAddr;
	int len = sizeof(clientAddr);
	while (1)
	{
		//연결 확인
		SOCKET client = accept(sock, (SOCKADDR*)&clientAddr, &len);
		if (client == INVALID_SOCKET)
		{
			break;
		}
		//inet_ntoa -> 네트워크 메모리 체계를 호스트의 메모리 체계로 변환
		printf("\n접속->%s:%d", inet_ntoa(clientAddr.sin_addr),
			ntohs(clientAddr.sin_port));
		bConnect = true;

		myMsg msg;
		//받는 버퍼
		char recvBuf[10000] = { 0, };
		char* pRecv = recvBuf;
		//받은 버퍼 크기
		int iRecvSize = 0;
		//보낼 버퍼 크기
		int iSendSize = 0;
		int iPacketSize = sizeof(myMsg);

		while (bConnect)
		{
			//클라이언트로부터 메시지를 받는다
			while (iRecvSize < iPacketSize)
			{
				iRecvSize += recv(client, recvBuf,
					iPacketSize - iRecvSize, 0);
				//메시지를 받지 못하거나 에러가 발생하면 루프 종료
				if (iRecvSize == 0 || iRecvSize == SOCKET_ERROR)
				{
					bConnect = false;
					break;
				}
			}
			//받은 메시지를 클라이언트로 돌려준다
			while (iSendSize < iPacketSize && bConnect)
			{
				memcpy(&msg, recvBuf, sizeof(myMsg));
				iSendSize += send(client, (char*)&msg,
					iPacketSize - iSendSize, 0);
				if (iSendSize == 0 || iSendSize == SOCKET_ERROR)
				{
					bConnect = false;
					break;
				}
			}
			iSendSize = 0;
			iRecvSize = 0;
			recvlist.push_back(msg);
			printf("\n%d:%s", msg.iCount, msg.buffer);
		}
		//클라이언트에게 셧다운 메시지를 보낸다
		shutdown(client, SD_SEND);
		//클라이언트의 소켓을 닫는다
		closesocket(client);
		printf("\n해제->%s:%d", inet_ntoa(clientAddr.sin_addr),
			ntohs(clientAddr.sin_port));
	}
	//서버의 소켓을 닫는다
	closesocket(sock);

	//WSA 해제
	WSACleanup();
	recvlist.clear();
}
