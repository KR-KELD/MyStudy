#pragma once
#include "myThread.h"
class myAcceptor : public myThread
{
public:
	SOCKET		 m_Sock;
public:
	virtual bool Run() override;
	virtual bool Accept();
	virtual bool AddSession(SOCKET sock, SOCKADDR_IN addr);
	virtual bool InitNetwork(std::string ip, int port);
	virtual bool DeleteNetwork();
	virtual bool InitSocket(std::string ip, int port);
	virtual bool CloseSocket();

public:
	myAcceptor();
	virtual ~myAcceptor();
};

