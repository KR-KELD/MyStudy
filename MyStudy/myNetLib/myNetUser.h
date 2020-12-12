#pragma once
#include "myNetStd.h"
#define MAX_BUFFER_SIZE 512
class myNetUser
{
public:
	SOCKET		m_Sock;
	SOCKADDR_IN addr;
	char		recvBuf[MAX_BUFFER_SIZE];
	int			iRecvSize;
	int			iSendSize;
	T_STR		szName;
	std::vector<UPACKET>	m_SendPacket;
	myNetUser();
	virtual ~myNetUser();
};

