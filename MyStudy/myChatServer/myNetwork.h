#pragma once
#include "myNetStd.h"

struct myMsg
{
	int  iCount;
	char buffer[3000];
};

const int g_iPacketSize = sizeof(myMsg);

struct myUser
{
	SOCKET		sock;
	SOCKADDR_IN	addr;
	char		recvBuf[10000];
	int			iRecvSize;
	int			iSendSize;
	myUser()
	{
		iRecvSize = 0;
		iSendSize = 0;
		ZeroMemory(recvBuf, _countof(recvBuf));
	}
};

class myNetwork
{
public:
	list<myUser>			m_listUser;
	vector<myMsg>			m_vecRecvPacket;
	list<myUser>::iterator	m_iterUser;
	SOCKET					m_ListenSocket;
public:
	bool			AddUser(SOCKET sock, SOCKADDR_IN addr);
	bool			DeleteUser(myUser& user);
	void			AddPacket(myUser& user);
	bool			RecvData(myUser& user);
	bool			SendData(myUser& user, myMsg& msg);
public:
	bool			InitNetwork();
	bool			Accept();
	bool			RecvDataList();
	bool			Broadcastting();
	bool			DeleteNetwork();

public:
	virtual bool	Run();
public:
	myNetwork();
	virtual ~myNetwork();
};

