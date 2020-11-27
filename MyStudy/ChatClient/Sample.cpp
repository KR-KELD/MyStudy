#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <stdio.h>
#include <tchar.h>
#include <time.h>
#include <conio.h>
#include <Ws2tcpip.h>	//inet_pton ���
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
	// ����Ʈ ���� ���� 	
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr("175.194.89.106");
	//error C4996 : 'inet_addr' : Use inet_pton() or InetPton() instead or define _WINSOCK_DEPRECATED_NO_WARNINGS
	sa.sin_port = htons(10000);
	iRet = connect(sock, (SOCKADDR*)&sa, sizeof(sa));
	if (iRet == SOCKET_ERROR)
	{
		return;
	}
	//�ͺ��ŷ �������� ��ȯ
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
		//Ű���� �Է� �ޱ�
		if (_kbhit() != 0)
		{
			//Ű �Է��� �ƽ�Ű�ڵ�� ����
			int iValue = _getche();
			//��������� ����
			int ilen = strlen(sendBuf);
			if (ilen == 0 && iValue == '\r')
			{
				memset(sendBuf, 0, sizeof(char) * 10000);
				continue;
			}
			//���͸� ġ�� ����
			if (iValue == '\r')
			{
				//���� ������ ������ �޽��� ����ü�� ��Ƽ� ������ ������
				memset(&msg, 0, sizeof(msg));
				strcpy_s(msg.buffer, sendBuf);
				msg.iCount = iCount++;

				iMsgLength = 0;
				//������ �ϳ��� ������ ������
				while (iSendSize < iPacketSize)
				{
					iSendSize += send(sock, (char*)&msg,
						iPacketSize - iSendSize, 0);
					//�����̰ų� ����޽����� ����������
					if (iSendSize == 0 || iSendSize == SOCKET_ERROR)
					{
						//�����ϰ��
						if (WSAGetLastError() != WSAEWOULDBLOCK)
						{
							bConnect = false;
						}
						break;
					}
				}
				//�����͸� ������ ������۸� �ʱ�ȭ
				memset(sendBuf, 0, sizeof(char) * 10000);
			}
			else
			{
				//���Ͱ� �ƴҰ��� ������۸� ä���
				sendBuf[iMsgLength++] = iValue;
			}
		}
		//���ú� ���� �ʱ�ȭ
		memset(recvBuf, 0, sizeof(char) * 10000);

		//������ �ϳ��� ������ �޾ƶ�
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