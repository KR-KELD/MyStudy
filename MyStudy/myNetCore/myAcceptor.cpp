#include "myAcceptor.h"
#include "myServer.h"
bool myAcceptor::InitNetwork(std::string ip, int port)
{
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

	if (InitSocket(ip, port) == false)
	{
		return false;
	}
	return true;
}
//���� �ʱ�ȭ
bool myAcceptor::InitSocket(std::string ip, int port)
{
	// �ͺ��ŷ �������� ��ȯ
	unsigned long iMode = 1;
	ioctlsocket(m_Sock, FIONBIO, &iMode);

	SOCKADDR_IN sa;
	// ����Ʈ ���� ���� 	
	sa.sin_family = AF_INET;

	sa.sin_addr.s_addr = htonl(INADDR_ANY);//inet_addr("175.194.89.26");// inet_addr("192.168.0.151");
	//error C4996 : 'inet_addr' : Use inet_pton() or InetPton() instead or define _WINSOCK_DEPRECATED_NO_WARNINGS
	sa.sin_port = htons(port);

	//SO_REUSEADDR�� ������ ������ ���� ���� ������ �ȴ�.
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
//��Ʈ��ũ ������
bool myAcceptor::DeleteNetwork()
{
	CloseSocket();
	return true;
}

bool myAcceptor::CloseSocket()
{
	closesocket(m_Sock);
	return true;
}

bool myAcceptor::Run()
{
	while (m_bStarted)
	{
		{
			myLock lock(this);
			if (Accept() == false) break;
		}
		Sleep(1);
	}
	return true;
}

bool myAcceptor::Accept()
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
		AddSession(client, clientAddr);
	}
	return true;
}

bool myAcceptor::AddSession(SOCKET sock, SOCKADDR_IN addr)
{
	{
		myLock lock(this);
		myNetUser* user = new myNetUser(m_pServer);
		user->m_Sock = sock;
		user->m_SockAddr = addr;
		m_pServer->m_SessionMgr.AddUser(user);
		user->WaitReceive();
		printf("\n����->%s:%d",
			inet_ntoa(addr.sin_addr),
			ntohs(addr.sin_port));
	}
	return true;
}

myAcceptor::myAcceptor(myServer* pServer) : m_pServer(pServer)
{
	// 2.2 ver
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
}

myAcceptor::~myAcceptor()
{
	DeleteNetwork();
	WSACleanup();
}
