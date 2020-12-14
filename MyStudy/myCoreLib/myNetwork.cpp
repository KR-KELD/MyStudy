#include "myNetwork.h"
#define NETWORK_MSG WM_USER + 80
bool myNetwork::g_bConnect = false;

bool myNetwork::SendPackets()
{
	std::vector<UPACKET>::iterator senditer;
	for (senditer = m_sendPacket.begin();
		senditer != m_sendPacket.end();
		senditer++)
	{
		if (SendData(*senditer) == false)
		{
			myNetwork::g_bConnect = false;
			return false;
		}
	}
	m_sendPacket.clear();
	return true;
}

void myNetwork::AddSendPacket(UPACKET& packet)
{
	m_sendPacket.push_back(packet);
}

void myNetwork::AddRecvPacket(myNetUser & user, UPACKET* packet)
{
	myPacket myPacket;
	myPacket.packet = *packet;
	myPacket.pUser = &user;

	m_recvPacket.push_back(myPacket);

	user.iRecvSize = 0;
}

bool myNetwork::MakePacket(UPACKET& packet, char* msg, int iLen, uint16_t type)
{
	packet.ph.iotype = 0;
	packet.ph.len = PACKET_HEADER_SIZE + iLen;
	packet.ph.type = type;
	packet.ph.time = time(NULL);
	memcpy(&packet.msg, msg, iLen);
	return true;
}

int myNetwork::SendMsg(SOCKET sock, char* msg, int iLen, uint16_t type)
{
	UPACKET packet;
	MakePacket(packet, msg, iLen, type);

	int iSendSize = 0;
	int iRet = 0;
	while (iSendSize < packet.ph.len)
	{
		iRet = send(sock, (char*)&packet,
			packet.ph.len - iSendSize, 0);
		if (iRet == 0 || iRet == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				myNetwork::g_bConnect = false;
				return -1;
			}
			return 0;
		}
		iSendSize += iRet;
	}
	return iSendSize;
}

bool myNetwork::RecvData()
{
	//�����Ͱ� ������ �޾ƶ�
	int iLen = recv(m_Sock, &m_User.recvBuf[m_User.iRecvSize],
		PACKET_HEADER_SIZE - m_User.iRecvSize, 0);
	//���� �����Ͱ� ������ �������� ������ ��쿡���� �����带 �����ϰų� ������
	//������ �����
	if (iLen == 0 || iLen == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			myNetwork::g_bConnect = false;
			return false;
		}
		return true;
	}
	m_User.iRecvSize += iLen;
	//������� �޴´�
	if (m_User.iRecvSize == PACKET_HEADER_SIZE)
	{
		UPACKET* packet = (UPACKET*)&m_User.recvBuf;
		//����� �� �޾����� ������ �����͸� �޴´�
		while (m_User.iRecvSize < packet->ph.len)
		{
			iLen = recv(m_Sock, &m_User.recvBuf[m_User.iRecvSize],
				packet->ph.len - m_User.iRecvSize, 0);
			if (iLen == 0 || iLen == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSAEWOULDBLOCK)
				{
					myNetwork::g_bConnect = false;
					return false;
				}
				return true;
			}
			m_User.iRecvSize += iLen;
		}
		//���� �� ��Ŷ ����Ʈ�� ��Ŷ �߰�
		AddRecvPacket(m_User, packet);
		//������ ���ú� ����,������ �ʱ�ȭ
		memset(m_User.recvBuf, 0, sizeof(char) * MAX_BUFFER_SIZE);
		m_User.iRecvSize = 0;
	}
	return true;
}

bool myNetwork::SendData(UPACKET& msg)
{
	while (m_User.iSendSize < msg.ph.len)
	{
		m_User.iSendSize += send(m_Sock,
			(char*)&msg,
			msg.ph.len - m_User.iSendSize, 0);
		if (m_User.iSendSize == 0)
		{
			return false;
		}
		if (m_User.iSendSize == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				myNetwork::g_bConnect = false;
				return false;
			}
			m_User.iSendSize = 0;
			break;
		}
	}
	if (m_User.iSendSize == msg.ph.len)
	{
		m_User.iSendSize = 0;
	}
	return true;
}

void myNetwork::SendLoginData(SOCKET sock, std::string id, std::string ps)
{
	myLogin login;
	strcpy_s(login.szID, id.c_str());
	strcpy_s(login.szPS, ps.c_str());
	SendMsg(sock, (char*)&login, myLoginSize, PACKET_LOGIN_REQ);
}

bool myNetwork::Frame()
{
	//����� ��Ʈ��ũ���� ������� Ȯ��
	int iIndex = WSAWaitForMultipleEvents(m_iArrayCount,
		m_EventArray, FALSE,
		0, FALSE);
	if (iIndex == WSA_WAIT_FAILED) return false;
	if (iIndex == WSA_WAIT_TIMEOUT) return true;
	//?
	iIndex -= WSA_WAIT_EVENT_0;

	WSANETWORKEVENTS NetworkEvent;
	//��Ʈ��ũ�鿡�� �̺�Ʈ�� �޴´�
	for (int iEvent = iIndex; iEvent < m_iArrayCount; iEvent++)
	{
		//��Ʈ��ũ�� �ϳ��� �ҷ��Ͷ�
		int iSignal = WSAWaitForMultipleEvents(1,
			&m_EventArray[iEvent], TRUE, 0, FALSE);
		if (iSignal == WSA_WAIT_FAILED)
		{
			myNetwork::g_bConnect = false;
			break;
		}
		if (iSignal == WSA_WAIT_TIMEOUT)
		{
			continue;
		}
		//�̺�Ʈ�� �޾ƿͶ�
		int iRet = WSAEnumNetworkEvents(m_Sock,
			m_EventArray[iEvent], &NetworkEvent);
		if (iRet == SOCKET_ERROR)
		{
			Check(iRet, __LINE__);
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return false;
			}
			break;
		}
		if (NetworkEvent.lNetworkEvents & FD_CONNECT)
		{
			if (NetworkEvent.iErrorCode[FD_CONNECT_BIT] != 0)
			{
				myNetwork::g_bConnect = false;
				return false;
			}
			myNetwork::g_bConnect = true;
			SendLoginData(m_Sock, "kgca", "game");
		}
		if (NetworkEvent.lNetworkEvents & FD_READ)
		{
			if (NetworkEvent.iErrorCode[FD_READ_BIT] != 0)
			{
				myNetwork::g_bConnect = false;
				continue;
			}
			if (RecvData() == false)
			{
				myNetwork::g_bConnect = false;
				continue;
			}
		}

		if (NetworkEvent.lNetworkEvents & FD_WRITE)
		{
			if (NetworkEvent.iErrorCode[FD_WRITE_BIT] != 0)
			{
				myNetwork::g_bConnect = false;
				return false;
			}
		}

		if (NetworkEvent.lNetworkEvents & FD_CLOSE)
		{
			if (NetworkEvent.iErrorCode[FD_CLOSE_BIT] != 0)
			{
				myNetwork::g_bConnect = false;
				return false;
			}
		}
	}
	return true;
}

bool  myNetwork::InitNetwork()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return false;
	}
	int iRet;
	m_Sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_Sock == INVALID_SOCKET)
	{
		Error(L"socket");
		return false;
	}
	// ���� Ÿ�� Ȯ��
	int sockType;
	int sockTypeLen = sizeof(int);
	getsockopt(m_Sock, SOL_SOCKET, SO_TYPE,
		(char*)&sockType, &sockTypeLen);
	if (sockType == SOCK_STREAM)
		printf("%s\r\n", "SOCK_STREAM.");
	else
		printf("%s\r\n", "SOCK_DGRAM");

	//�ۼ��� ���� ũ�� Ȯ��	
	int iSendBuffer = 100000;
	setsockopt(m_Sock, SOL_SOCKET, SO_SNDBUF,
		(char*)&iSendBuffer, sockTypeLen);

	getsockopt(m_Sock, SOL_SOCKET, SO_SNDBUF,
		(char*)&sockType, &sockTypeLen);
	printf("SendBuffer=%d\n", sockType);
	getsockopt(m_Sock, SOL_SOCKET, SO_RCVBUF,
		(char*)&sockType, &sockTypeLen);
	printf("RecvBuffer=%d\n", sockType);


	int optval = 1;
	// 1���� ���μ��������� ip&port ���� ����
	// bind�Լ����� ������ ��´�.
	Check(
		setsockopt(m_Sock, SOL_SOCKET, SO_EXCLUSIVEADDRUSE,
		(char*)&optval, sizeof(optval)),
		__LINE__
	);
	// �ټ� ���μ������� ip&port ���� ����
	/*iRet = setsockopt(m_Sock, SOL_SOCKET, SO_REUSEADDR,
		(char*)&optval, sizeof(optval));
	if (iRet == SOCKET_ERROR)
	{
		return false;
	}*/

	linger optLinger;
	int iLingerLen = sizeof(optLinger);
	optLinger.l_onoff = 1;
	optLinger.l_linger = 1000;
	if (setsockopt(m_Sock, SOL_SOCKET, SO_LINGER,
		(char*)&optLinger, iLingerLen) != 0)
	{
		Error(L"SO_LINGER");
		return false;
	}
	if (InitSocket() == false)
	{
		return false;
	}

	unsigned long iMode = 1;
	ioctlsocket(m_Sock, FIONBIO, &iMode);

	//myNetwork::g_bConnect = true;
	//SendLoginData(m_Sock, "kgca", "game");
	return true;
}

bool  myNetwork::InitSocket()
{
	//��Ʈ��ũ �߰�
	m_EventArray[m_iArrayCount] = WSACreateEvent();
	WSAEventSelect(m_Sock, m_EventArray[m_iArrayCount],
		FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);
	//��Ʈ��ũ ī��Ʈ ����
	m_iArrayCount++;
	return true;
}

bool myNetwork::ConnectServer(std::string ip, int port)
{
	SOCKADDR_IN sa;
	// ����Ʈ ���� ���� 	
	USHORT jValue = 10000;
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr(ip.c_str());
	//error C4996 : 'inet_addr' : Use inet_pton() or InetPton() instead or define _WINSOCK_DEPRECATED_NO_WARNINGS
	sa.sin_port = htons(port);
	int iRet = connect(m_Sock, (SOCKADDR*)&sa, sizeof(sa));
	//iRet = WSAConnect(m_Sock, (SOCKADDR*)&sa, sizeof(sa),
	//					NULL,NULL,NULL,NULL);
	if (iRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			return false;
		}
	}
}

bool myNetwork::DeleteNetwork()
{
	closesocket(m_Sock);
	WSACleanup();
	return true;
}

myNetwork::myNetwork()
{
	m_User.strName = L"defalt";
	m_bLogin = false;
}

myNetwork::~myNetwork()
{

}