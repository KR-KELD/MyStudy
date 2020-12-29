#include "myLoginServer.h"
#include "myLoginAcceptor.h"
bool myLoginServer::Init()
{
	myServer::Init();
	m_IOCP = new myIOCP(this);
	m_Acceptor = new myLoginAcceptor(this);
	m_SessionMgr.Init(this);
	m_IOCP->Init();
	m_Acceptor->InitNetwork("", 10000);
	m_Acceptor->CreateThread();
	CreateThread();
	m_bRun = true;

	//m_fnExecutePacket[PACKET_CHAT_MSG] = &myServer::PacketChatMsg;
	//m_fnExecutePacket[PACKET_USER_POSITION] = &myServer::PacketUserPos;
	//m_fnExecutePacket[PACKET_LOGIN_REQ] = &myServer::PacketLoginLeq;
	//m_fnExecutePacket[PACKET_LOGOUT_PLAYER] = &myServer::PacketLogoutPlayer;
	return true;
}

bool myLoginServer::Release()
{
	myServer::Release();
	m_IOCP->Release();
	delete m_Acceptor;
	delete m_IOCP;
	return true;
}

myLoginServer::myLoginServer() : m_bRun(false)
{
}

myLoginServer::~myLoginServer()
{

}
