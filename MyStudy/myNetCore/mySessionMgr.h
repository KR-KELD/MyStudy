#pragma once
#include "myIOCP.h"
class myServer;
class mySessionMgr :/* public NetSingleTon<mySessionMgr>, */public myServerObj
{
//private:
//	friend NetSingleTon<mySessionMgr>;
public:
	myServer*								m_pServer;
	std::map<SOCKET, myNetUser*>			m_UserList;
	std::map<SOCKET, myNetUser*>::iterator	m_UserIter;
public:
	virtual bool	Init(myServer* pServer);
	virtual void	AddUser(myNetUser * pUser);
	virtual bool	DelUser(myNetUser* pUser);
	virtual bool	CloseUser(myNetUser* pUser);
	virtual bool	MoveOtherSession(mySessionMgr& other, myNetUser* pUser);
	//void			Lock();
	//void			UnLock();

public:
	mySessionMgr();
	virtual ~mySessionMgr();
};
//#define I_Session mySessionMgr::GetInstance()
