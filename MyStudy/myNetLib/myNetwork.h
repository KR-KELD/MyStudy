#pragma once
#include "myNetUser.h"


struct myPacket
{
	myNetUser*  pUser;
	UPACKET packet;
};

class myNetwork
{
public:
	myNetUser				m_User;
	SOCKET					m_Sock;
	std::vector<myPacket>	m_recvPacket;
	static bool				g_bConnect;
public:
	void	SendLoginData(SOCKET sock, std::string id, std::string ps);
	bool	MakePacket(UPACKET& packet, char* msg, int iLen, uint16_t type);
	int		SendMsg(SOCKET sock, char* msg, int iLen, uint16_t type);
	void	AddPacket(myNetUser& user, UPACKET* packet);
	void	MsgEvent(MSG msg);
	virtual bool	RecvData();
	virtual bool	SendData(myNetUser& user, UPACKET& msg);

public:
	virtual bool	InitNetwork(std::string ip, int port);
	virtual bool	InitSocket(std::string ip, int port);
	virtual bool	DeleteNetwork();
	virtual void	PacketProcess();
};

