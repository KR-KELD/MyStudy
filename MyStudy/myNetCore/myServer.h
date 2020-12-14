#pragma once
#include "mySessionMgr.h"

class myServer :public NetSingleTon<myServer>, public myThread
{
private:
	friend class NetSingleTon<myServer>;
public:

public:
	myServer();
	virtual ~myServer();
};
#define I_Server myServer::GetInstance();
