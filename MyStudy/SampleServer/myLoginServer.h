#pragma once
#include "myAcceptor.h"
class myLoginServer
{
public:
	myAcceptor	m_Acceptor;
	bool		m_bRun;
public:
	bool		Init();
	bool		Run();
	bool		Release();
public:
	myLoginServer();
	virtual ~myLoginServer();
};

