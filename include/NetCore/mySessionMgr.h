#pragma once
#include "myNetUser.h"

class mySessionMgr :/* public NetSingleTon<mySessionMgr>, */public myServerObj
{
//private:
//	friend NetSingleTon<mySessionMgr>;
public:
	std::map<SOCKET, myNetUser*>		m_UserList;
public:
	void			AddUser(myNetUser * pUser);
	virtual bool	DelUser(myNetUser* pUser);
	virtual bool	CloseUser(myNetUser* user);
	void			Lock();
	void			UnLock();

public:
	mySessionMgr();
	virtual ~mySessionMgr();
};
//#define I_Session mySessionMgr::GetInstance()
