#pragma once
#include "myNetUser.h"

class mySessionMgr : public myServerObj
{
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

