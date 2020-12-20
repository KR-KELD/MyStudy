#include "mySessionMgr.h"

void mySessionMgr::AddUser(myNetUser * pUser)
{
	EnterCriticalSection(&m_cs);
		m_UserList[pUser->m_Sock] = pUser;
	LeaveCriticalSection(&m_cs);
}

bool mySessionMgr::DelUser(myNetUser* pUser)
{
	EnterCriticalSection(&m_cs);
		printf("\nÇØÁ¦->%s:%d",
			inet_ntoa(pUser->m_SockAddr.sin_addr),
			ntohs(pUser->m_SockAddr.sin_port));

		std::map<SOCKET, myNetUser*>::iterator iter =
			m_UserList.find(pUser->m_Sock);
		if (iter != m_UserList.end())
		{
			CloseUser(pUser);
			m_UserList.erase(iter);
		}
	LeaveCriticalSection(&m_cs);
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

bool mySessionMgr::MoveOtherSession(mySessionMgr & other, myNetUser * pUser)
{
	EnterCriticalSection(&m_cs);
	std::map<SOCKET, myNetUser*>::iterator iter =
		m_UserList.find(pUser->m_Sock);
	if (iter != m_UserList.end())
	{
		m_UserList.erase(iter);
		other.m_UserList[pUser->m_Sock] = pUser;
	}
	LeaveCriticalSection(&m_cs);
	return true;
}

//void mySessionMgr::Lock()
//{
//	EnterCriticalSection(&m_cs);
//}
//
//void mySessionMgr::UnLock()
//{
//	LeaveCriticalSection(&m_cs);
//}

mySessionMgr::mySessionMgr()
{

}

mySessionMgr::~mySessionMgr()
{

}
