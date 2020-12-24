#pragma once
#include "myPacketPool.h"
#include "mySessionMgr.h"
#include "myIOCP.h"
#include "myAcceptor.h"

class myServer : /*public NetSingleTon<myServer>,*/ public myThread
{
private:
	friend class NetSingleTon<myServer>;
public:
	myPacketPool	m_RecvPacketPool;
	myPacketPool	m_SendPacketPool;
	myPacketPool	m_SendBroadcastPacketPool;
public:
	mySessionMgr	m_SessionMgr;	//����� ��� ���� ����
	myIOCP*			m_IOCP;
	myAcceptor*		m_Acceptor;
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
	virtual  bool Release();
public:
	myServer();
	virtual ~myServer();
};
//#define I_Server myServer::GetInstance()
