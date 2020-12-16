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
//public:
//	mySessionMgr	m_SessionMgr;
public:
	typedef	void (myServer::*CallFuction)(myPacket t);
	typedef std::map<int, CallFuction>::iterator FunctionIterator;
	std::map<int, CallFuction>    m_fnExecutePacket;
public:
	void	PacketChatMsg(myPacket packet);
	void	PacketLoginLeq(myPacket packet);
	void	PacketUserPos(myPacket packet);
public:
	//virtual  bool SendData(myNetUser& user, UPACKET& msg);
	virtual  bool MakePacket(UPACKET& packet, char* msg, int iLen, uint16_t type);
	virtual  bool Run() override;
	virtual  bool Broadcastting();
	virtual  bool Init();
public:
	myServer();
	virtual ~myServer();
};
#define I_Server myServer::GetInstance()
