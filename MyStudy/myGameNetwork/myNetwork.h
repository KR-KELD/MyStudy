#pragma once
#include "myUser.h"

//유저 정보를 가지고 있는 패킷
struct myPacket
{
	//유저
	myUser*  pUser;
	UPACKET packet;
};


class myNetwork
{
public:
	//유저 리스트
	std::list<myUser>		m_UserList;
	//소켓
	SOCKET					m_Sock;
	//유저로부터 받은 패킷
	std::vector<myPacket>	m_recvPacket;
	//임계구역 변수
	CRITICAL_SECTION		m_cs;
public:
	bool	MakePacket(UPACKET& packet,
		char* msg, int iLen, uint16_t type);
	bool	AddUser(SOCKET sock, SOCKADDR_IN addr);
	bool	DelUser(myUser& user);
	void	AddPacket(myUser& user, UPACKET* packet);
	virtual bool	RecvData(myUser& user);
	bool	SendData(myUser& user, UPACKET& msg);
	int		SendMsg(SOCKET sock, char* msg, int iLen, uint16_t type);
public:
	virtual bool Run();
	virtual bool RecvUserList();
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

