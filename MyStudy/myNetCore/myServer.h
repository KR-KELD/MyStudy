#pragma once
#include "myPacketPool.h"
#include "myAcceptor.h"
#include <functional>

class myServer : /*public NetSingleTon<myServer>,*/ public myThread
{
private:
	friend class NetSingleTon<myServer>;
public:
	myPacketPool	m_RecvPacketPool;
	myPacketPool	m_SendPacketPool;
	myPacketPool	m_SendBroadcastPacketPool;
public:
	mySessionMgr	m_SessionMgr;	//楷搬等 葛电 蜡历 包府
	myIOCP*			m_IOCP;
	myAcceptor*		m_Acceptor;
//public:
//	typedef	void (myServer::*CallFuction)(myPacket& t);
//	typedef std::map<int, CallFuction>::iterator FunctionIterator;
//	std::map<int, CallFuction>    m_fnExecutePacket;
public:
	typedef std::function<void(myPacket& t)> CallFunc;
	typedef std::map<int, CallFunc>::iterator FuncIterator;
	std::map<int, CallFunc>    m_fnExecuteHandler;
	FuncIterator IterCallFunc;
public:
	void	PacketChatMsg(myPacket& packet);
	void	PacketLoginLeq(myPacket& packet);
	void	PacketUserPos(myPacket& packet);
	void	PacketLogoutPlayer(myPacket& packet);
public:

public:
	virtual	 void SendLogoutUser(myNetUser* pUser);
	virtual  bool MakePacket(UPACKET& packet, char* msg, int iLen, uint16_t type);
	virtual  bool PreRun();
	virtual  bool Run() override;
	virtual  bool PostRun();
	virtual  bool Broadcastting();
	virtual  bool Init();
	virtual  bool Release();
public:
	myServer();
	virtual ~myServer();
};
//#define I_Server myServer::GetInstance()
