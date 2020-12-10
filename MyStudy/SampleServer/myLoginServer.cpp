#include "myLoginServer.h"
#include "myIOCP.h"
bool myLoginServer::Init()
{
	I_Iocp.Init();
	m_Acceptor.InitNetwork("175.194.89.26",10000);
	m_Acceptor.CreateThread();
	m_bRun = true;
	return true;
}

bool myLoginServer::Run()
{
	while (m_bRun)
	{
		Sleep(10);
	}
	WaitForSingleObject((HANDLE)m_Acceptor.m_hThread, INFINITE);
	return true;
}

bool myLoginServer::Release()
{
	return true;
}

myLoginServer::myLoginServer() : m_bRun(false)
{
}

myLoginServer::~myLoginServer()
{
	I_Iocp.Release();
}
