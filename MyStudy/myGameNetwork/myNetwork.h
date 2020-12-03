#pragma once
#include "myUser.h"

//���� ������ ������ �ִ� ��Ŷ
struct myPacket
{
	//����
	myUser*  pUser;
	UPACKET packet;
};


class myNetwork
{
public:
	//���� ����Ʈ
	std::list<myUser>		m_UserList;
	//����
	SOCKET					m_Sock;
	//�����κ��� ���� ��Ŷ
	std::vector<myPacket>	m_recvPacket;
	//�Ӱ豸�� ����
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

