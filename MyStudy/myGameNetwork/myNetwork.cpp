#include "myNetwork.h"
//쓰레드 주의할점
//스위치를 하면 다른 쓰레드로 작업핸들이 넘어간다
//동기화를 쓰는 이유 : static 메모리, heap 메모리에 들어있는 데이터를
//공유하게 될 경우 값이 자기 멋대로 바뀔 수가 있다
//이유는 쓰레드의 작업 순서가 순차적이 아니기때문
//쓰레드의 동기화에도 문제가 발생하는데
//두 개 이상의 쓰레드가 한 임계구역에 물릴경우
//서로 접근을 못하게되어 멈추는 현상 deadlock에 걸리게 된다
//때문에 경우의 수를 확실하게 체크하고 막아야 한다


//수정하기 내 입맛대로

//패킷생성
bool myNetwork::MakePacket(UPACKET& packet,
	char* msg, int iLen, uint16_t type)
{
	packet.ph.iotype = 0;
	packet.ph.len = PACKET_HEADER_SIZE + iLen;
	packet.ph.type = type;
	packet.ph.time = time(NULL);
	memcpy(&packet.msg, msg, iLen);
	return true;
}
//메시지 전송
int myNetwork::SendMsg(SOCKET sock, char* msg, int iLen, uint16_t type)
{
	//패킷 생성
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
//데이터 수신
bool myNetwork::RecvData(myNetUser& user)
{
	//데이터가 들어오면 받아라
	int iLen = recv(user.m_Sock, &user.recvBuf[user.iRecvSize],
		PACKET_HEADER_SIZE - user.iRecvSize, 0);
	//받은 데이터가 있으면 내려가고 없으면 경우에따라 쓰레드를 종료하거나 유저의
	//연결을 끊어라
	if (iLen == 0 || iLen == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			return false;
		}
		return true;
	}
	user.iRecvSize += iLen;
	//헤더부터 받는다
	if (user.iRecvSize == PACKET_HEADER_SIZE)
	{
		UPACKET* packet = (UPACKET*)&user.recvBuf;
		//헤더를 다 받았으면 나머지 데이터를 받는다
		while (user.iRecvSize < packet->ph.len)
		{
			iLen = recv(user.m_Sock, &user.recvBuf[user.iRecvSize],
				packet->ph.len - user.iRecvSize, 0);
			if (iLen == 0 || iLen == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSAEWOULDBLOCK)
				{
					return false;
				}
				return true;
			}
			user.iRecvSize += iLen;
		}
		//전송 할 패킷 리스트에 패킷 추가
		AddPacket(user, packet);
		//유저의 리시브 버퍼,사이즈 초기화
		memset(user.recvBuf, 0, sizeof(char) * 10000);
		user.iRecvSize = 0;
	}
	return true;
}
//유저에게 데이터 전송
bool myNetwork::SendData(myNetUser& user, UPACKET& msg)
{
	//패킷 데이터 전송
	while (user.iSendSize < msg.ph.len)
	{
		user.iSendSize += send(user.m_Sock,
			(char*)&msg,
			msg.ph.len - user.iSendSize, 0);
		
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
	if (user.iSendSize == msg.ph.len)
	{
		user.iSendSize = 0;
	}
	return true;
}
//유저로부터 데이터 받아오기
bool myNetwork::RecvDataList()
{
	std::list<myNetUser>::iterator iter;
	for (iter = m_UserList.begin();
		iter != m_UserList.end();
		)
	{
		if (RecvData(*iter) == false)
		{
			DelUser(*iter);
			iter = m_UserList.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	return true;
}
//유저로부터 전달받은 메시지를 패킷화
void myNetwork::PacketProcess()
{
	std::vector<myPacket>::iterator senditer;
	for (senditer = m_recvPacket.begin();
		senditer != m_recvPacket.end();
		senditer++)
	{
		UPACKET* packet = (UPACKET*)&senditer->packet;
		if (packet->ph.type == PACKET_CHAT_MSG)
		{
			TChatMsg* pMsg = (TChatMsg*)&packet->msg;
			printf("\n[%s]%s:%d", pMsg->szName,
				pMsg->buffer, pMsg->iCnt);
		}
		if (packet->ph.type == PACKET_LOGIN_REQ)
		{
			UPACKET sendPacket;
			T_STR szID = L"kgca";
			T_STR szPS = L"game";
			TLogin* login = (TLogin*)packet->msg;
			T_STR szIDUser = to_mw(login->szID);
			T_STR szPSUser = to_mw(login->szPS);
			TLoginResult ret;
			if (szID == szIDUser && szPS == szPSUser)
			{
				ret.iRet = 1;
			}
			else
			{
				ret.iRet = 0;
			}
			MakePacket(sendPacket, (char*)&ret, sizeof(TLoginResult),
				PACKET_LOGIN_ACK);
			senditer->pUser->m_SendPacket.push_back(sendPacket);
		}
	}
}
bool myNetwork::Process()
{
	PacketProcess();

	std::list<myNetUser>::iterator iter;
	for (iter = m_UserList.begin();
		iter != m_UserList.end();
		)
	{
		bool bDelete = false;
		//각자에게 저장된 메시지를 각자에게 전달
		std::vector<UPACKET>::iterator senditer;
		for (senditer = iter->m_SendPacket.begin();
			senditer != iter->m_SendPacket.end();
			senditer++)
		{
			if (SendData(*iter, *senditer) == false)
			{
				bDelete == true;
				break;
			}
		}
		iter->m_SendPacket.clear();

		if (bDelete == true)
		{
			DelUser(*iter);
			iter = m_UserList.erase(iter);
		}
		else
		{
			iter++;
		}

	}

	return Broadcastting();
}
//리스트에 저장된 메시지를 모든 유저에게 전달
bool myNetwork::Broadcastting()
{
	std::list<myNetUser>::iterator iter;
	for (iter = m_UserList.begin();
		iter != m_UserList.end();
		)
	{
		bool bDelete = false;
		std::vector<myPacket>::iterator senditer;
		for (senditer = m_recvPacket.begin();
			senditer != m_recvPacket.end();
			senditer++)
		{
			if (SendData(*iter, senditer->packet) == false)
			{
				bDelete == true;
				break;
			}
		}
		if (bDelete == true)
		{
			DelUser(*iter);
			iter = m_UserList.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	m_recvPacket.clear();
	return true;
}
bool myNetwork::InitNetwork(std::string ip, int port)
{
	InitializeCriticalSection(&m_cs);
	// 2.2 ver
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

	// 넌블로킹 소켓으로 전환
	unsigned long iMode = 1;
	ioctlsocket(m_Sock, FIONBIO, &iMode);

	return true;
}
//소켓 초기화
bool myNetwork::InitSocket(std::string ip, int port)
{
	SOCKADDR_IN sa;
	// 바이트 정렬 구조 	
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = htonl(INADDR_ANY);// inet_addr("192.168.0.151");
	//error C4996 : 'inet_addr' : Use inet_pton() or InetPton() instead or define _WINSOCK_DEPRECATED_NO_WARNINGS
	sa.sin_port = htons(port);

	//SO_REUSEADDR가 설정된 소켓이 있을 경우는 오류가 된다.
	Check(
		bind(m_Sock, (SOCKADDR*)&sa, sizeof(sa)),
		__LINE__
	);
	int iRet = listen(m_Sock, SOMAXCONN);
	if (iRet == SOCKET_ERROR)
	{
		Error(L"listen");
		return false;
	}
	return true;
}
//네트워크 릴리즈
bool myNetwork::DeleteNetwork()
{
	closesocket(m_Sock);
	WSACleanup();

	DeleteCriticalSection(&m_cs);
	return true;
}
//리스트에 유저 추가
bool myNetwork::AddUser(SOCKET sock, SOCKADDR_IN addr)
{
	myNetUser user;
	user.m_Sock = sock;
	user.addr = addr;
	m_UserList.push_back(user);
	printf("\n접속->%s:%d",
		inet_ntoa(addr.sin_addr),
		ntohs(addr.sin_port));
	return true;
}
//리스트에서 유저 제거
bool myNetwork::DelUser(myNetUser& user)
{
	shutdown(user.m_Sock, SD_SEND);
	closesocket(user.m_Sock);
	printf("\n해제->%s:%d",
		inet_ntoa(user.addr.sin_addr),
		ntohs(user.addr.sin_port));
	return true;
}
//전송할 패킷 리스트에 추가
void myNetwork::AddPacket(myNetUser & user, UPACKET* packet)
{
	myPacket myPacket;
	myPacket.packet = *packet;
	myPacket.pUser = &user;
	m_recvPacket.push_back(myPacket);
	user.iRecvSize = 0;
}
bool myNetwork::Accept()
{
	SOCKADDR_IN clientAddr;
	int len = sizeof(clientAddr);

	SOCKET client = accept(m_Sock, (SOCKADDR*)&clientAddr, &len);
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
bool myNetwork::Run()
{
	while (1)
	{
		if (Accept() == false)
		{
			break;
		}
		if (RecvDataList() == false)
		{
			break;
		}
		if (Process() == false)
		{
			break;
		}
	}
	return true;
}
myNetwork::myNetwork() {}
myNetwork::~myNetwork() {}