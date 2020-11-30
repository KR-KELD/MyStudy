#include "myNetwork.h"

bool myNetwork::AddUser(SOCKET sock, SOCKADDR_IN addr)
{
	myUser user;
	user.sock = sock;
	user.addr = addr;
	m_listUser.push_back(user);
	printf("\n����->%s:%d", inet_ntoa(addr.sin_addr),
		ntohs(addr.sin_port));
	return true;
}

bool myNetwork::DeleteUser(myUser & user)
{
	shutdown(user.sock, SD_SEND);
	closesocket(user.sock);
	printf("\n����->%s:%d", inet_ntoa(user.addr.sin_addr),
		ntohs(user.addr.sin_port));
	return true;
}

void myNetwork::AddPacket(myUser & user)
{
	myMsg msg;
	memcpy(&msg, user.recvBuf, sizeof(myMsg));
	m_vecRecvPacket.push_back(msg);
	user.iRecvSize = 0;
}

bool myNetwork::RecvData(myUser & user)
{
	while (user.iRecvSize < g_iPacketSize)
	{
		user.iRecvSize += recv(user.sock, user.recvBuf,
			g_iPacketSize - user.iRecvSize, 0);
		if (user.iRecvSize == 0)
		{
			return false;
		}
		if (user.iRecvSize == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return false;
			}
			user.iRecvSize = 0;
			break;
		}
	}
	if (user.iRecvSize == g_iPacketSize)
	{
		AddPacket(user);
	}
	return true;
}

bool myNetwork::SendData(myUser & user, myMsg & msg)
{
	while (user.iSendSize < g_iPacketSize)
	{
		user.iSendSize += send(user.sock,
			(char*)&msg,
			g_iPacketSize - user.iSendSize, 0);
		if (user.iSendSize == 0)
		{
			return false;
		}
		if (user.iSendSize == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return false;
			}
			user.iSendSize = 0;
			break;
		}
	}
	if (user.iSendSize == g_iPacketSize)
	{
		user.iSendSize = 0;
	}
}

bool myNetwork::InitNetwork()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return false;
	}
	int iRet;
	//TCP-IP
	m_ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_ListenSocket == INVALID_SOCKET)
	{
		Error(L"SOCKET");
		return false;
	}
	//���� Ÿ�� Ȯ��
	int iSockType;
	//������ Ÿ�� ����
	int iSockTypeLen = sizeof(int);
	//���� �ɼ� ��ȸ-���� Ÿ��
	getsockopt(m_ListenSocket, SOL_SOCKET, SO_TYPE,
		(char*)&iSockType, &iSockTypeLen);
	//TCP���� UDP����
	if (iSockType == SOCK_STREAM)
		printf("%s\r\n", "SOCK_STREAM.");
	else
		printf("%s\r\n", "SOCK_DGRAM");

	//�ۼ��� ���� ũ�� Ȯ��	
	int iSendBuffer = 100000;
	//���� �ɼ� ����-������� ũ��
	Check(setsockopt(m_ListenSocket, SOL_SOCKET, SO_SNDBUF,
		(char*)&iSendBuffer, sizeof(iSendBuffer)), __LINE__);
	//���� �ɼ� ��ȸ-������� ũ��
	getsockopt(m_ListenSocket, SOL_SOCKET, SO_SNDBUF,
		(char*)&iSockType, &iSockTypeLen);
	printf("SendBuffer=%d\n", iSockType);
	//���� �ɼ� ��ȸ-���ú���� ũ��
	getsockopt(m_ListenSocket, SOL_SOCKET, SO_RCVBUF,
		(char*)&iSockType, &iSockTypeLen);
	printf("RecvBuffer=%d\n", iSockType);
	int iOptValue = 1;
	Check(setsockopt(m_ListenSocket, SOL_SOCKET, SO_EXCLUSIVEADDRUSE,
		(char*)&iOptValue, sizeof(iOptValue)), __LINE__);

	//Ŭ���̾�Ʈ�� ������ ������ ��
	//���ð��� �ִ� �ɼ�
	linger optLinger;
	int iLingerLen = sizeof(optLinger);
	//����
	optLinger.l_onoff = 1;
	//�ð�
	optLinger.l_linger = 1000;
	Check(setsockopt(m_ListenSocket, SOL_SOCKET, SO_LINGER,
		(char*)&optLinger, iLingerLen), __LINE__);
	//�ּ�
	//����Ʈ ���� ���� 	
	SOCKADDR_IN sa;
	//��Ʈ ��ȣ
	USHORT jValue = 10000;
	sa.sin_family = AF_INET;
	//ip�ּ�
	//��� ��巹���� �ްڴ�
	//inet_addr(INADDR_ANY); 
	//inet_addr("175.194.89.106");
	sa.sin_addr.s_addr = inet_addr("192.168.0.169");
	//error C4996 : 'inet_addr' : Use inet_pton() or InetPton() instead or define _WINSOCK_DEPRECATED_NO_WARNINGS
	//htons -> ȣ��Ʈ�� �޸� ü�迡�� ��Ʈ��ũ�� �޸� ü��� ��ȯ �� �ִ� �Լ�
	sa.sin_port = htons(10000);

	//SO_REUSEADDR�� ������ ������ ���� ���� ������ �ȴ�.
	Check(bind(m_ListenSocket, (SOCKADDR*)&sa, sizeof(sa)), __LINE__);

	//������ ��û�� ���� �� �ִ� ���·� ����� �ش�.
	Check(listen(m_ListenSocket, SOMAXCONN), __LINE__);

	// �ͺ��ŷ �������� ��ȯ
	unsigned long iMode = 1;
	Check(ioctlsocket(m_ListenSocket, FIONBIO, &iMode), __LINE__);

	return true;
}

bool myNetwork::Accept()
{
	//Ŭ���̾�Ʈ �ּ�
	SOCKADDR_IN clientAddr;
	int len = sizeof(clientAddr);

	SOCKET client = accept(m_ListenSocket, (SOCKADDR*)&clientAddr, &len);
	if (client == INVALID_SOCKET)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			return false;
		}
	}
	else
	{
		AddUser(client, clientAddr);
	}
	return true;
}

bool myNetwork::RecvDataList()
{
	for (m_iterUser = m_listUser.begin();
		m_iterUser != m_listUser.end();
		)
	{
		if (RecvData(*m_iterUser) == false)
		{
			DeleteUser(*m_iterUser);
			m_iterUser = m_listUser.erase(m_iterUser);
		}
		else
		{
			m_iterUser++;
		}
	}
	return true;
}

bool myNetwork::Broadcastting()
{
	for (m_iterUser = m_listUser.begin();
		m_iterUser != m_listUser.end();
		)
	{
		bool bDelete = false;
		//�� �� �غ� ���� ������ ���ͷ����ͷ� �ٲٱ�
		for(int i = 0;i< m_vecRecvPacket.size();i++)
		{
			if (SendData(*m_iterUser, m_vecRecvPacket[i]) == false)
			{
				bDelete == true;
				break;
			}
		}
		if (bDelete == true)
		{
			DeleteUser(*m_iterUser);
			m_iterUser = m_listUser.erase(m_iterUser);
		}
		else
		{
			m_iterUser++;
		}
	}
	m_vecRecvPacket.clear();
	return true;
}

bool myNetwork::DeleteNetwork()
{
	closesocket(m_ListenSocket);
	WSACleanup();
	return true;
}

bool myNetwork::Run()
{
	while (true)
	{
		if (Accept() == false)
		{
			break;
		}
		if (RecvDataList() == false)
		{
			break;
		}
		if (Broadcastting() == false)
		{
			break;
		}
	}
	return true;
}

myNetwork::myNetwork()
{
}

myNetwork::~myNetwork()
{
}
