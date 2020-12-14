#pragma once
#include "myNetwork.h"
#include "myCore.h"
#include "myDraw.h"

class Sample : public myCore
{
	myNetwork	m_Net;
	std::map<int, void(Sample::*)(myPacket&)>	m_vecPacketFunc;
	HANDLE		m_hThread;
public:
	static string		strServerIP;
	void  PacketChatMsg(myPacket& packet);
	void  PacketLoginAck(myPacket& packet);
public:
	void  LoginSeq();
	void  SendTest(const char* pData);
public:
	bool  Init();
	bool  Frame();
	bool  Render();
	bool  Release();
};
