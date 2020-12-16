#pragma once
#include "myServerObj.h"
#include "myObjectPool.h"
#define MAX_BUFFER_SIZE 512
#define MAX_DATA_BUFFER_SIZE  (MAX_BUFFER_SIZE * 3)

class myNetUser;
struct myPacket
{
	myNetUser*  pUser;
	UPACKET packet;
};

struct OVERLAPPED2 : myObjectPool<OVERLAPPED2>
{
	enum { MODE_RECV = 1, MODE_SEND = 2, MODE_EXIT };
	OVERLAPPED ov;
	int	 iType; // 0:recv, 1:send
	OVERLAPPED2(int type)
	{
		memset(&ov, 0, sizeof(OVERLAPPED));
		iType = type;
	}
	OVERLAPPED2()
	{
		memset(&ov, 0, sizeof(OVERLAPPED));
		iType = MODE_RECV;
	}
};

class myNetUser
{
public:
	bool				 m_bExit;	//���� ���� �÷���
	SOCKET				 m_Sock;	//����
	SOCKADDR_IN			 m_SockAddr;	//�ּ�	
	char				 m_szRecvBuffer[MAX_BUFFER_SIZE];	//���ú����
	char				 m_szSendBuffer[MAX_BUFFER_SIZE];	//�������
	WSABUF				 m_wsaRecvBuffer;	//wsa���ú� ����
	WSABUF				 m_wsaSendBuffer;	//wsa���� ����
	OVERLAPPED2			 m_ovRead;	//�������� ����
	OVERLAPPED2			 m_ovSend;	//�������� ����
	T_STR				 szName;	//�����̸�
	//std::vector<UPACKET> m_SendPacket;	//���������� ���� ��Ŷ
public:
	char	m_szDataBuffer[MAX_DATA_BUFFER_SIZE];	//������ ����
	int		m_iPacketPos; // ��Ŷ�� ���� �ּ�
	int		m_iWritePos; // ��Ŷ ������ ���� �ּ�
	int		m_iReadPos; // ��Ŷ ������ ���� �ּ�
public:
	virtual bool DispatchRead(DWORD dwTrans, OVERLAPPED2* ov);
	virtual bool DispatchWrite(DWORD dwTrans, OVERLAPPED2* ov);

	OVERLAPPED2* OverlappedRecv(int type);
	OVERLAPPED2* OverlappedSend(int type, UPACKET& msg);

	virtual bool WaitReceive();
	virtual bool WaitSend(UPACKET& msg);
public:
	myNetUser();
	virtual ~myNetUser();
};

