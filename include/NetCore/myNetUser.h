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
	enum { MODE_RECV = 1, MODE_SEND = 2, MODE_EXIT};
	int		iType; // 0:recv , 1:send
};

class myNetUser
{
public:
	bool				 m_bExit;	//접속 여부 플래그
	SOCKET				 m_Sock;	//소켓
	SOCKADDR_IN			 m_SockAddr;	//주소	
	char				 m_szRecvBuffer[MAX_BUFFER_SIZE];	//리시브버퍼
	char				 m_szSendBuffer[MAX_BUFFER_SIZE];	//샌드버퍼
	WSABUF				 m_wsaRecvBuffer;	//wsa리시브 버퍼
	WSABUF				 m_wsaSendBuffer;	//wsa샌드 버퍼
	OVERLAPPED2			 m_ovRead;	//오버랩드 리드
	OVERLAPPED2			 m_ovSend;	//오버랩드 샌드
	T_STR				 szName;	//유저이름
	std::vector<UPACKET> m_SendPacket;	//개인적으로 보낼 패킷
public:
	char	m_szDataBuffer[MAX_DATA_BUFFER_SIZE];	//데이터 버퍼
	int		m_iPacketPos; // 패킷의 시작 주소
	int		m_iWritePos; // 패킷 데이터 저장 주소
	int		m_iReadPos; // 패킷 데이터 저장 주소
public:
	virtual bool DispatchRead(DWORD dwTrans, OVERLAPPED2* ov);
	virtual bool DispatchWrite(DWORD dwTrans, OVERLAPPED2* ov);
	virtual bool WaitReceive();
public:
	myNetUser();
	virtual ~myNetUser();
};

