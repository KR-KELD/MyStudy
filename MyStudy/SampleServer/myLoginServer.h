#pragma once
#include "myLoginAcceptor.h"
class myLoginServer
{
public:
	myLoginAcceptor	m_Acceptor;
	bool			m_bRun;
public:
	bool			Init();
	bool			Run();
	bool			Release();
public:
	myLoginServer();
	virtual ~myLoginServer();
};

