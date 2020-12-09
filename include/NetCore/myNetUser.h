#pragma once
#include "myServerObj.h"
#define MAX_BUFFER_SIZE 512
#define MAX_DATA_BUFFER_SIZE  (MAX_BUFFER_SIZE * 3)

class myNetUser;
struct myPacket
{
	myNetUser*  pUser;
	UPACKET packet;
};

struct OVERLAPPED2 : OVERLAPPED
{
	enum { MODE_RECV = 1, MODE_SEND = 2 };
	int		iType; // 0:recv , 1:send
};

class myNetUser
{
public:
	SOCKET		m_Sock;						//����
	SOCKADDR_IN addr;						//�ּ�	
	char		recvBuf[MAX_BUFFER_SIZE];				//���ú����
	WSABUF		m_wsaRecvBuffer;
	WSABUF		m_wsaSendBuffer;
	int			iRecvSize;					//���ú������
	int			iSendSize;					//���������
	OVERLAPPED2	m_ovRead;
	OVERLAPPED2	m_ovSend;
	T_STR		szName;						//�����̸�
	std::vector<UPACKET>	m_SendPacket;	//���������� ���� ��Ŷ
public:
	char	m_szRecvBuffer[MAX_DATA_BUFFER_SIZE];	//������ ����
	int		m_iPacketPos = 0; // ��Ŷ�� ���� �ּ�
	int		m_iWritePos = 0; // ��Ŷ ������ ���� �ּ�
	int		m_iReadPos = 0; // ��Ŷ ������ ���� �ּ�
public:
	virtual bool DispatchRead(DWORD dwTrans, OVERLAPPED2* ov);
	virtual bool DispatchWrite(DWORD dwTrans, OVERLAPPED2* ov);
	virtual bool WaitReceive();
public:
	myNetUser();
	virtual ~myNetUser();
};

