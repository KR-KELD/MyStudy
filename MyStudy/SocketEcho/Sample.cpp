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
//����
void main()
{
	// 2.2 ver
	WSADATA wsa;
	//WSA2.2���� ����
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}
	//���� �޽��� ����Ʈ
	vector<myMsg> recvlist;

	int iRet;
	//���� ���� TCPŸ��
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
	{
		//Check(L"Socket");
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
	setsockopt(sock, SOL_SOCKET, SO_SNDBUF,
		(char*)&iSendBuffer, sizeof(iSendBuffer));
	//���� �ɼ� ��ȸ-������� ũ��
	getsockopt(sock, SOL_SOCKET, SO_SNDBUF,
		(char*)&iSockType, &iSockTypeLen);
	printf("SendBuffer=%d\n", iSockType);
	//���� �ɼ� ��ȸ-���ú���� ũ��
	getsockopt(sock, SOL_SOCKET, SO_RCVBUF,
		(char*)&iSockType, &iSockTypeLen);
	printf("RecvBuffer=%d\n", iSockType);


	int iOptValue = 1;

	//���� �ɼ� ����
	//���μ������� 1���� ip�� port�� ���� �����ϴ�
	//bind �Լ����� ��� ����
	iRet = setsockopt(sock, SOL_SOCKET, SO_EXCLUSIVEADDRUSE,
		(char*)&iOptValue, sizeof(iOptValue));
	if (iRet == SOCKET_ERROR)
	{
		return;
	}
	//���� ���μ������� ������ �����ϴ�
	/*iRet = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,
		(char*)&optval, sizeof(optval));
	if (iRet == SOCKET_ERROR)
	{
		return;
	}*/

	//Ŭ���̾�Ʈ�� ������ ������ ��
	//���ð��� �ִ� �ɼ�
	linger optLinger;
	int iLingerLen = sizeof(optLinger);
	//����
	optLinger.l_onoff = 1;
	//�ð�
	optLinger.l_linger = 1000;
	if (setsockopt(sock, SOL_SOCKET, SO_LINGER,
		(char*)&optLinger, iLingerLen) != 0)
	{
		return;
	}
	//�ּ�
	//����Ʈ ���� ���� 	
	SOCKADDR_IN sa;
	//��Ʈ ��ȣ
	USHORT jValue = 10000;
	sa.sin_family = AF_INET;
	//ip�ּ�
	sa.sin_addr.s_addr = inet_addr("192.168.0.169");
	//error C4996 : 'inet_addr' : Use inet_pton() or InetPton() instead or define _WINSOCK_DEPRECATED_NO_WARNINGS
	//htons -> ȣ��Ʈ�� �޸� ü�迡�� ��Ʈ��ũ�� �޸� ü��� ��ȯ �� �ִ� �Լ�
	sa.sin_port = htons(10000);

	//SO_REUSEADDR�� ������ ������ ���� ���� ������ �ȴ�.
	iRet = bind(sock, (SOCKADDR*)&sa, sizeof(sa));
	if (iRet == SOCKET_ERROR)
	{
		return;
	}
	//������ ��û�� ���� �� �ִ� ���·� ����� �ش�.
	iRet = listen(sock, SOMAXCONN);
	bool bConnect = false;
	//Ŭ���̾�Ʈ �ּ�
	SOCKADDR_IN clientAddr;
	int len = sizeof(clientAddr);
	while (1)
	{
		//���� Ȯ��
		SOCKET client = accept(sock, (SOCKADDR*)&clientAddr, &len);
		if (client == INVALID_SOCKET)
		{
			break;
		}
		//inet_ntoa -> ��Ʈ��ũ �޸� ü�踦 ȣ��Ʈ�� �޸� ü��� ��ȯ
		printf("\n����->%s:%d", inet_ntoa(clientAddr.sin_addr),
			ntohs(clientAddr.sin_port));
		bConnect = true;

		myMsg msg;
		//�޴� ����
		char recvBuf[10000] = { 0, };
		char* pRecv = recvBuf;
		//���� ���� ũ��
		int iRecvSize = 0;
		//���� ���� ũ��
		int iSendSize = 0;
		int iPacketSize = sizeof(myMsg);

		while (bConnect)
		{
			//Ŭ���̾�Ʈ�κ��� �޽����� �޴´�
			while (iRecvSize < iPacketSize)
			{
				iRecvSize += recv(client, recvBuf,
					iPacketSize - iRecvSize, 0);
				//�޽����� ���� ���ϰų� ������ �߻��ϸ� ���� ����
				if (iRecvSize == 0 || iRecvSize == SOCKET_ERROR)
				{
					bConnect = false;
					break;
				}
			}
			//���� �޽����� Ŭ���̾�Ʈ�� �����ش�
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
		//Ŭ���̾�Ʈ���� �˴ٿ� �޽����� ������
		shutdown(client, SD_SEND);
		//Ŭ���̾�Ʈ�� ������ �ݴ´�
		closesocket(client);
		printf("\n����->%s:%d", inet_ntoa(clientAddr.sin_addr),
			ntohs(clientAddr.sin_port));
	}
	//������ ������ �ݴ´�
	closesocket(sock);

	//WSA ����
	WSACleanup();
	recvlist.clear();
}
