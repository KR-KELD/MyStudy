#include "mySessionMgr.h"
#include "myLock.h"

void mySessionMgr::AddUser(myNetUser * pUser)
{
	{
		myLock lock(this);
		this->m_UserList[pUser->m_Sock] = pUser;
	}
}

bool mySessionMgr::DelUser(myNetUser* pUser)
{
	{
		myLock lock(this);
		printf("\nÇØÁ¦->%s:%d",
			inet_ntoa(pUser->m_SockAddr.sin_addr),
			ntohs(pUser->m_SockAddr.sin_port));

		CloseUser(pUser);
		std::map<SOCKET, myNetUser*>::iterator iter =
			m_UserList.find(pUser->m_Sock);
		if (iter != m_UserList.end())
		{
			m_UserList.erase(iter);
		}
	}
	return true;
}

bool mySessionMgr::CloseUser(myNetUser* pUser)
{
	if (pUser == nullptr) return true;
	shutdown(pUser->m_Sock, SD_SEND);
	closesocket(pUser->m_Sock);
	delete pUser;
	pUser = nullptr;
	return true;
}

void mySessionMgr::Lock()
{
	EnterCriticalSection(&m_cs);
}

void mySessionMgr::UnLock()
{
	LeaveCriticalSection(&m_cs);
}

mySessionMgr::mySessionMgr()
{

}

mySessionMgr::~mySessionMgr()
{

}
