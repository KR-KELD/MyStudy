#pragma once
#include "myNetStd.h"

struct OVERLAPPED2 : OVERLAPPED
{
	int		iType; // 0:recv , 1:send
};

class myNetUser
{
public:
	SOCKET		m_Sock;						//����
	SOCKADDR_IN addr;						//�ּ�	
	char		recvBuf[10000];				//���ú����
	int			iRecvSize;					//���ú������
	int			iSendSize;					//���������
	T_STR		szName;						//�����̸�
	std::vector<UPACKET>	m_SendPacket;	//���������� ���� ��Ŷ
	myNetUser();
	virtual ~myNetUser();
};

