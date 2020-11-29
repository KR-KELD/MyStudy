#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <stdio.h>
#include <tchar.h>
#include <time.h>
#include <string>
#include <vector>
#include <list>
#include <Ws2tcpip.h>	//inet_pton ���
#pragma comment (lib, "ws2_32.lib")
using namespace std;

struct myMsg
{
	int  iCount;
	char buffer[3000];
};

void Error(const TCHAR* szHeader = L"ERROR", const TCHAR* szMsg = L"\0")
{
	LPVOID* lpMsg = 0;;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(TCHAR*)lpMsg, 0, NULL);
	wstring szBuffer = szMsg;
	szBuffer += L"\n";
	szBuffer += (TCHAR*)lpMsg;
	MessageBox(NULL, szBuffer.c_str(), szHeader, MB_ICONERROR);
	LocalFree(lpMsg);
}

void Check(int iRet, int line)
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

struct myUser
{
	SOCKET			sock;
	SOCKADDR_IN		addr;
	vector<myMsg>	msgList;
	bool			bConnect;
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
		Error(L"SOCKET");
		return;
	}
	//���� Ÿ�� Ȯ��
	int iSockType;
	//������ Ÿ�� ����
	int iSockTypeLen = sizeof(int);
	//���� �ɼ� ��ȸ-���� Ÿ��
	getsockopt(sock, SOL_SOCKET, SO_TYPE,
		(char*)&iSockType, &iSockTypeLen);
	//TCP���� UDP����
	if (iSockType == SOCK_STREAM)
		printf("%s\r\n", "SOCK_STREAM.");
	else
		printf("%s\r\n", "SOCK_DGRAM");

	//�ۼ��� ���� ũ�� Ȯ��	
	int iSendBuffer = 100000;
	//���� �ɼ� ����-������� ũ��
	Check(setsockopt(sock, SOL_SOCKET, SO_SNDBUF,
		(char*)&iSendBuffer, sizeof(iSendBuffer)),__LINE__);
	//���� �ɼ� ��ȸ-������� ũ��
	getsockopt(sock, SOL_SOCKET, SO_SNDBUF,
		(char*)&iSockType, &iSockTypeLen);
	printf("SendBuffer=%d\n", iSockType);
	//���� �ɼ� ��ȸ-���ú���� ũ��
	getsockopt(sock, SOL_SOCKET, SO_RCVBUF,
		(char*)&iSockType, &iSockTypeLen);
	printf("RecvBuffer=%d\n", iSockType);
	int iOptValue = 1;
	Check(setsockopt(sock, SOL_SOCKET, SO_EXCLUSIVEADDRUSE,
		(char*)&iOptValue, sizeof(iOptValue)),__LINE__);

	//Ŭ���̾�Ʈ�� ������ ������ ��
	//���ð��� �ִ� �ɼ�
	linger optLinger;
	int iLingerLen = sizeof(optLinger);
	//����
	optLinger.l_onoff = 1;
	//�ð�
	optLinger.l_linger = 1000;
	Check(setsockopt(sock, SOL_SOCKET, SO_LINGER,
		(char*)&optLinger, iLingerLen), __LINE__);
	//�ּ�
	//����Ʈ ���� ���� 	
	SOCKADDR_IN sa;
	//��Ʈ ��ȣ
	USHORT jValue = 10000;
	sa.sin_family = AF_INET;
	//ip�ּ�
	//��� ��巹���� �ްڴ�
	sa.sin_addr.s_addr = inet_addr("175.194.89.106"); //inet_addr(INADDR_ANY);
	//error C4996 : 'inet_addr' : Use inet_pton() or InetPton() instead or define _WINSOCK_DEPRECATED_NO_WARNINGS
	//htons -> ȣ��Ʈ�� �޸� ü�迡�� ��Ʈ��ũ�� �޸� ü��� ��ȯ �� �ִ� �Լ�
	sa.sin_port = htons(10000);

	//SO_REUSEADDR�� ������ ������ ���� ���� ������ �ȴ�.
	Check(bind(sock, (SOCKADDR*)&sa, sizeof(sa)), __LINE__);

	//������ ��û�� ���� �� �ִ� ���·� ����� �ش�.
	Check(listen(sock, SOMAXCONN), __LINE__);

	//Ŭ���̾�Ʈ �ּ�
	SOCKADDR_IN clientAddr;
	int len = sizeof(clientAddr);

	// �ͺ��ŷ �������� ��ȯ
	unsigned long iMode = 1;
	Check(ioctlsocket(sock, FIONBIO, &iMode), __LINE__);

	list<SOCKET> userList;
	list<SOCKET>::iterator userIter;
	bool bConnect = false;
	while (true)
	{
		//���� Ȯ��
		SOCKET client = accept(sock, (SOCKADDR*)&clientAddr, &len);
		if (client == INVALID_SOCKET)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				continue;
			}
		}
		else
		{
			userList.push_back(client);
			printf("\n����->%s:%d", inet_ntoa(clientAddr.sin_addr),
				ntohs(clientAddr.sin_port));
		}
		myMsg msg;
		//�޴� ����
		char recvBuf[10000] = { 0, };
		//���� ���� ũ��
		int iRecvSize = 0;
		//���� ���� ũ��
		int iSendSize = 0;
		int iPacketSize = sizeof(myMsg);

		for (userIter = userList.begin();
			userIter != userList.end();)
		{
			bConnect = true;
			while (iRecvSize < iPacketSize)
			{
				iRecvSize += recv(*userIter, recvBuf,
					iPacketSize - iRecvSize, 0);
				if (iRecvSize == 0 || iRecvSize == SOCKET_ERROR)
				{
					if (WSAGetLastError() != WSAEWOULDBLOCK)
					{
						bConnect = false;
						shutdown(*userIter, SD_SEND);
						closesocket(*userIter);
						userIter = userList.erase(userIter);
					}
					break;
				}
			}
			if (bConnect == false || iRecvSize == -1)
			{
				iSendSize = 0;
				iRecvSize = 0;
				continue;
			}

			while (iSendSize < iPacketSize)
			{
				memcpy(&msg, recvBuf, sizeof(myMsg));
				iSendSize += send(*userIter, (char*)&msg,
					iPacketSize - iSendSize, 0);
				if (iSendSize == 0 || iSendSize == SOCKET_ERROR)
				{
					if (WSAGetLastError() != WSAEWOULDBLOCK)
					{
						bConnect = false;
						shutdown(*userIter, SD_SEND);
						closesocket(*userIter);
						userIter = userList.erase(userIter);
					}
					break;
				}
			}
			if (bConnect == false)
			{
				iSendSize = 0;
				iRecvSize = 0;
				continue;
			}
			iSendSize = 0;
			iRecvSize = 0;
			userIter++;
		}
	}
	closesocket(sock);
	WSACleanup();
}