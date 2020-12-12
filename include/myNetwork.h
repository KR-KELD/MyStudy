#pragma once
#include "myNetUser.h"
#include "myStd.h"


struct myPacket
{
	myNetUser*  pUser;
	UPACKET packet;
};

class myNetwork
{
private:
	HANDLE		m_EventArray[WSA_MAXIMUM_WAIT_EVENTS];
	int			m_iArrayCount;
public:
	myNetUser				m_User;
	SOCKET					m_Sock;
	std::vector<UPACKET>	m_sendPacket;
	std::vector<myPacket>	m_recvPacket;
	bool					m_bLogin;
	static bool				g_bConnect;
public:
	bool	SendPackets();
	void	AddSendPacket(UPACKET& packet);
	void	AddRecvPacket(myNetUser& user, UPACKET* packet);
	bool	MakePacket(UPACKET& packet, char* msg, int iLen, uint16_t type);
	int		SendMsg(SOCKET sock, char* msg, int iLen, uint16_t type);

	bool	RecvData();
	bool	SendData(UPACKET& msg);
	void	SendLoginData(SOCKET sock, std::string id, std::string ps);
	//void	MsgEvent(MSG msg);
public:
	virtual bool	Frame();
public:
	virtual bool	InitNetwork(std::string ip, int port);
	virtual bool	InitSocket(std::string ip, int port);
	virtual bool	DeleteNetwork();
public:
	myNetwork();
	virtual ~myNetwork();
};

