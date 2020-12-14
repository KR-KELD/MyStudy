#pragma once
#include "myNetwork.h"
#include "myCore.h"
class Sample
{
	myNetwork	m_Net;
public:
	string		m_strChat;
	std::vector<myNetUser>	m_vecUser;
	std::map<int, void(Sample::*)(myPacket&)>	m_mapPacketFunc;
	std::list<string>		m_listMsg;
public:
	float	m_fGameTimer;
	float	m_fSecondPerFrame;
	int		m_iFPS;
	float	m_fBeforeTime;
public:
	void  PacketChatMsg(myPacket& packet);
	void  PacketLoginAck(myPacket& packet);
	void  LoginSeq();
	void  SendMsg(const char* pData);
	void  MsgProcess();
public:
public:
	void  Timer();
public:
	bool  Init();
	bool  Frame();
	bool  Render();
	bool  Release();
};

