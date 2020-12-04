#pragma once
#include "myNetUser.h"

//���� ������ ������ �ִ� ��Ŷ
struct myPacket
{
	//����
	myNetUser*  pUser;
	UPACKET packet;
};


class myNetwork
{
public:
	//���� ����Ʈ
	std::list<myNetUser>		m_UserList;
	//����
	SOCKET					m_Sock;
	//�����κ��� ���� ��Ŷ
	std::vector<myPacket>	m_recvPacket;
	//�Ӱ豸�� ����
	CRITICAL_SECTION		m_cs;
public:
	//�ʼ�
	bool	MakePacket(UPACKET& packet,
		char* msg, int iLen, uint16_t type);
	//�ʼ�
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

