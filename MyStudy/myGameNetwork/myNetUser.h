#pragma once
#include "myNetStd.h"
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

