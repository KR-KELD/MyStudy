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
		//iocp에 핸들엔 소켓 키엔 유저 주소값을 넘겨서 연결
		user->WaitReceive();
		//아마도 스레드가 첫 결과를 확인 할 수 있게 하기위해 연결과 동시에 리시브를 한 번 호출
		printf("\n접속->%s:%d\n",
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
