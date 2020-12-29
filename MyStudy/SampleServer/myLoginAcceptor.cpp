#include "myLoginAcceptor.h"
#include "myServer.h"

bool myLoginAcceptor::AddSession(SOCKET sock, SOCKADDR_IN addr)
{
	{
		myLock lock(this);
		myChatUser* user = new myChatUser(m_pServer);
		user->m_Sock = sock;
		user->m_SockAddr = addr;
		m_pServer->m_SessionMgr.AddUser(user);
		//iocp�� �ڵ鿣 ���� Ű�� ���� �ּҰ��� �Ѱܼ� ����
		user->WaitReceive();
		//�Ƹ��� �����尡 ù ����� Ȯ�� �� �� �ְ� �ϱ����� ����� ���ÿ� ���ú긦 �� �� ȣ��
		printf("\n����->%s:%d\n",
			inet_ntoa(addr.sin_addr),
			ntohs(addr.sin_port));
	}
	return true;
}

myLoginAcceptor::myLoginAcceptor(myServer* pServer) : myAcceptor(pServer)
{
	m_pServer = pServer;
}

myLoginAcceptor::~myLoginAcceptor()
{
}
