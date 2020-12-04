#pragma once
#include "myNetUser.h"

//유저 정보를 가지고 있는 패킷
struct myPacket
{
	//유저
	myNetUser*  pUser;
	UPACKET packet;
};


class myNetwork
{
public:
	//유저 리스트
	std::list<myNetUser>		m_UserList;
	//소켓
	SOCKET					m_Sock;
	//유저로부터 받은 패킷
	std::vector<myPacket>	m_recvPacket;
	//임계구역 변수
	CRITICAL_SECTION		m_cs;
public:
	//필수
	bool	MakePacket(UPACKET& packet,
		char* msg, int iLen, uint16_t type);
	//필수
	bool	AddUser(SOCKET sock, SOCKADDR_IN addr);
	bool	DelUser(myNetUser& user);
	void	AddPacket(myNetUser& user, UPACKET* packet);
	virtual bool	RecvData(myNetUser& user);
	bool	SendData(myNetUser& user, UPACKET& msg);
	int		SendMsg(SOCKET sock, char* msg, int iLen, uint16_t type);
public:
	virtual bool Run();
	virtual bool RecvDataList();
	virtual bool Broadcastting();
	virtual bool InitNetwork(std::string ip, int iPort);
	virtual bool InitSocket(std::string ip, int iPort);
	virtual bool DeleteNetwork();
	virtual bool Accept();
	virtual bool Process();
	virtual void PacketProcess();
public:
	myNetwork();
	virtual ~myNetwork();
};

