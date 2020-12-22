#pragma once
#include "mySessionMgr.h"
#include "myPacketPool.h"
#include "myThread.h"

class myServer : public NetSingleTon<myServer>, public myThread
{
private:
	friend class NetSingleTon<myServer>;
public:
	myPacketPool	m_RecvPacketPool;
	myPacketPool	m_SendPacketPool;
	myPacketPool	m_SendBroadcastPacketPool;
public:
	mySessionMgr	m_SessionMgr;	//楷搬等 葛电 蜡历 包府
public:
	typedef	void (myServer::*CallFuction)(myPacket& t);
	typedef std::map<int, CallFuction>::iterator FunctionIterator;
	std::map<int, CallFuction>    m_fnExecutePacket;
public:
	void	PacketChatMsg(myPacket& packet);
	void	PacketLoginLeq(myPacket& packet);
	void	PacketUserPos(myPacket& packet);
	void	PacketLogoutPlayer(myPacket& packet);
public:
	virtual	 void SendLogoutUser(myNetUser* pUser);
	virtual  bool MakePacket(UPACKET& packet, char* msg, int iLen, uint16_t type);
	virtual  bool Run() override;
	virtual  bool Broadcastting();
	virtual  bool Init();
public:
	myServer();
	virtual ~myServer();
};
#define I_Server myServer::GetInstance()
