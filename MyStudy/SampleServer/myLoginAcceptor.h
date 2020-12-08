#pragma once
#include "myAcceptor.h"
class myLoginAcceptor : public myAcceptor
{
public:
	virtual bool Run() override;
	virtual bool AddSession(SOCKET sock, SOCKADDR_IN addr) override;
};

