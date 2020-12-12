#include "myNetwork.h"
#define NETWORK_MSG WM_USER+80
bool myNetwork::g_bConnect = false;

void myNetwork::SendLoginData(SOCKET sock, std::string id, std::string ps)
{
	myLogin login;
	strcpy_s(login.szID, id.c_str());
	strcpy_s(login.szPS, ps.c_str());
	SendMsg(sock, (char*)&login, myLoginSize, PACKET_LOGIN_REQ);
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
				return -1;
			}
			return 0;
		}
		iSendSize += iRet;
	}
	return iSendSize;
}

void myNetwork::AddPacket(myNetUser & user, UPACKET* packet)
{
	myPacket myPacket;
	myPacket.packet = *packet;
	myPacket.pUser = &user;

	m_recvPacket.push_back(myPacket);

	user.iRecvSize = 0;
}

void myNetwork::MsgEvent(MSG msg)
{
	switch (msg.message)
	{
	case NETWORK_MSG:
	{
		if (WSAGETSELECTERROR(msg.lParam) != 0)
		{
			//PostQuitMessage(0);
			myNetwork::g_bConnect = false;
			return;
		}
		WORD dwSelect = WSAGETSELECTEVENT(msg.lParam);
		switch (dwSelect)
		{
		case FD_CONNECT:
		{
			myNetwork::g_bConnect = true;
			SendLoginData(m_Sock, "kgca", "game");
		}break;
		case FD_CLOSE:
		{
			myNetwork::g_bConnect = false;
		}break;
		case FD_READ:
		{
			RecvData();
			//PostMessage(g_hWnd, NETWORK_MSG, m_Sock, FD_READ);
		}break;
		case FD_WRITE:
		{

		}break;
		}
	}break;
	}
}

bool myNetwork::RecvData()
{
	// header
	if (m_User.iRecvSize < PACKET_HEADER_SIZE)
	{
		m_User.iRecvSize += recv(m_Sock, &m_User.recvBuf[m_User.iRecvSize],
			PACKET_HEADER_SIZE - m_User.iRecvSize, 0);
		if (m_User.iRecvSize == 0 || m_User.iRecvSize == SOCKET_ERROR)
		{
			m_User.iRecvSize = 0;
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return false;
			}
			return true;
		}
	}
	else
	{
		UPACKET* packet = (UPACKET*)&m_User.recvBuf;
		m_User.iRecvSize += recv(m_Sock, &m_User.recvBuf[m_User.iRecvSize],
			packet->ph.len - m_User.iRecvSize, 0);
		if (m_User.iRecvSize == 0 || m_User.iRecvSize == SOCKET_ERROR)
		{
			m_User.iRecvSize = 0;
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return false;
			}
			return true;
		}
		if (m_User.iRecvSize == packet->ph.len)
		{
			AddPacket(m_User, packet);
			memset(m_User.recvBuf, 0, sizeof(char) * 10000);
			m_User.iRecvSize = 0;
		}
	}
	return true;
}

bool  myNetwork::InitNetwork(std::string ip, int port)
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
	// 소켓 타입 확인
	int sockType;
	int sockTypeLen = sizeof(int);
	getsockopt(m_Sock, SOL_SOCKET, SO_TYPE,
		(char*)&sockType, &sockTypeLen);
	if (sockType == SOCK_STREAM)
		printf("%s\r\n", "SOCK_STREAM.");
	else
		printf("%s\r\n", "SOCK_DGRAM");

	//송수신 버퍼 크기 확인	
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
	// 1개의 프로세스에서만 ip&port 재사용 가능
	// bind함수에서 오류를 얻는다.
	Check(
		setsockopt(m_Sock, SOL_SOCKET, SO_EXCLUSIVEADDRUSE,
		(char*)&optval, sizeof(optval)),
		__LINE__
	);
	// 다수 프로세스에서 ip&port 재사용 가능
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

	if (InitSocket(ip, port) == false)
	{
		return false;
	}

	myNetwork::g_bConnect = true;
	SendLoginData(m_Sock, "kgca", "game");
	return true;
}

bool  myNetwork::InitSocket(std::string ip, int port)
{
	// 넌블로킹소켓 : 네트워크 이벤트
	int iRet = WSAAsyncSelect(m_Sock,
		g_hWnd, NETWORK_MSG,
		FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);
	if (iRet == SOCKET_ERROR) return false;

	SOCKADDR_IN sa;
	// 바이트 정렬 구조 	
	USHORT jValue = 10000;
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr(ip.c_str());
	//error C4996 : 'inet_addr' : Use inet_pton() or InetPton() instead or define _WINSOCK_DEPRECATED_NO_WARNINGS
	sa.sin_port = htons(port);
	iRet = connect(m_Sock, (SOCKADDR*)&sa, sizeof(sa));
	//iRet = WSAConnect(m_Sock, (SOCKADDR*)&sa, sizeof(sa),
	//					NULL,NULL,NULL,NULL);
	if (iRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			return false;
		}
	}
	return true;
}

bool myNetwork::DeleteNetwork()
{
	closesocket(m_Sock);
	WSACleanup();
	return true;
}

void  myNetwork::PacketProcess()
{

}