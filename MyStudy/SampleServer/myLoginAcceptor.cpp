#include "myLoginAcceptor.h"

bool myLoginAcceptor::Run()
{
	while (m_bStarted)
	{
		{
			myLock lock(this);
			if (Accept() == false) break;
		}
	}
	return true;
}

bool myLoginAcceptor::AddSession(SOCKET sock, SOCKADDR_IN addr)
{
	//myNetUser user;
	//user.m_Sock = sock;
	//user.addr = addr;
	
	//EnterCriticalSection(&m_cs);
	//m_UserList.push_back(user);
	//LeaveCriticalSection(&m_cs);
	
	//printf("\nÁ¢¼Ó->%s:%d",
	//	inet_ntoa(addr.sin_addr),
	//	ntohs(addr.sin_port));
	return true;
}
