#include "mySessionMgr.h"
#include "myServer.h"
bool mySessionMgr::Init(myServer * pServer)
{
	m_pServer = pServer;
	return true;
}
void mySessionMgr::AddUser(myNetUser * pUser)
{
	EnterCriticalSection(&m_cs);
		m_UserList[pUser->m_Sock] = pUser;
		m_pServer->m_IOCP->SetBind(pUser->m_Sock, (ULONG_PTR)pUser);
	LeaveCriticalSection(&m_cs);
}

bool mySessionMgr::DelUser(myNetUser* pUser)
{
	EnterCriticalSection(&m_cs);
		printf("\nÇØÁ¦->%s:%d",
			inet_ntoa(pUser->m_SockAddr.sin_addr),
			ntohs(pUser->m_SockAddr.sin_port));

		m_UserIter = m_UserList.find(pUser->m_Sock);
		if (m_UserIter != m_UserList.end())
		{
			CloseUser(pUser);
			m_UserList.erase(m_UserIter);
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
	m_UserIter = m_UserList.find(pUser->m_Sock);
	if (m_UserIter != m_UserList.end())
	{
		m_UserList.erase(m_UserIter);
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
	EnterCriticalSection(&m_cs);
	for (m_UserIter = m_UserList.begin();
		m_UserIter != m_UserList.end();
		m_UserIter++)
	{
		CloseUser(m_UserIter->second);
	}
	m_UserList.clear();
	LeaveCriticalSection(&m_cs);
}
