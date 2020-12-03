#pragma once
#include "myNetStd.h"
class myUser
{
public:
	SOCKET		m_Sock;						//소켓
	SOCKADDR_IN addr;						//주소	
	char		recvBuf[10000];				//리시브버퍼
	int			iRecvSize;					//리시브사이즈
	int			iSendSize;					//샌드사이즈
	T_STR		szName;						//유저이름
	std::vector<UPACKET>	m_SendPacket;	//개인적으로 보낼 패킷
	myUser();
	virtual ~myUser();
};

