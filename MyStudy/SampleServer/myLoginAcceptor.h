#pragma once
#include "myAcceptor.h"
#include "myChatUser.h"
class myLoginAcceptor : public myAcceptor
{
public:
	virtual bool AddSession(SOCKET sock, SOCKADDR_IN addr) override;
	myLoginAcceptor(myServer* pServer);
	virtual ~myLoginAcceptor();

};

