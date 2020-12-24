#pragma once
#include "myThread.h"
class myServer;

class myAcceptor : public myThread
{
public:
	SOCKET			m_Sock;
	myServer*		m_pServer;
public:
	virtual bool Run() override;
	virtual void GetServer(myServer* pServer);
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

