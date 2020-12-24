#pragma once
#include "myServer.h"
class myLoginServer : public myServer
{
public:
	bool			m_bRun;
public:
	bool			Init();
	bool			Release();
public:
	void	PacketChatMsg(myPacket& packet);
	void	PacketLoginLeq(myPacket& packet);
	void	PacketUserPos(myPacket& packet);
	void	PacketLogoutPlayer(myPacket& packet);
public:
	myLoginServer();
	virtual ~myLoginServer();
};

