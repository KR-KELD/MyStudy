#pragma once
#include "myAcceptor.h"
#include "myChatUser.h"
class myLoginAcceptor : public myAcceptor
{
	std::vector<myPacket>	m_recvPacket;
public:
	virtual bool Run() override;
	virtual bool AddSession(SOCKET sock, SOCKADDR_IN addr) override;
};

