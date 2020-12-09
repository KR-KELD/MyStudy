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
	SOCKET		m_Sock;						//소켓
	SOCKADDR_IN addr;						//주소	
	char		recvBuf[MAX_BUFFER_SIZE];				//리시브버퍼
	WSABUF		m_wsaRecvBuffer;
	WSABUF		m_wsaSendBuffer;
	int			iRecvSize;					//리시브사이즈
	int			iSendSize;					//샌드사이즈
	OVERLAPPED2	m_ovRead;
	OVERLAPPED2	m_ovSend;
	T_STR		szName;						//유저이름
	std::vector<UPACKET>	m_SendPacket;	//개인적으로 보낼 패킷
public:
	char	m_szRecvBuffer[MAX_DATA_BUFFER_SIZE];	//데이터 버퍼
	int		m_iPacketPos = 0; // 패킷의 시작 주소
	int		m_iWritePos = 0; // 패킷 데이터 저장 주소
	int		m_iReadPos = 0; // 패킷 데이터 저장 주소
public:
	virtual bool DispatchRead(DWORD dwTrans, OVERLAPPED2* ov);
	virtual bool DispatchWrite(DWORD dwTrans, OVERLAPPED2* ov);
	virtual bool WaitReceive();
public:
	myNetUser();
	virtual ~myNetUser();
};

