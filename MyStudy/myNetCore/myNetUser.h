#pragma once
#include "myServerObj.h"
#include "myObjectPool.h"
#define MAX_BUFFER_SIZE 512
#define MAX_DATA_BUFFER_SIZE  (MAX_BUFFER_SIZE * 3)

class myServer;
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
	//std::vector<UPACKET> m_SendPacket;	//개인적으로 보낼 패킷
public:
	char	m_szDataBuffer[MAX_DATA_BUFFER_SIZE];	//데이터 버퍼
	int		m_iPacketPos; // 패킷의 시작 주소
	int		m_iWritePos; // 패킷 데이터 저장 주소
	int		m_iReadPos; // 패킷 데이터 저장 주소
public:
	virtual bool DispatchRead(myServer* pServer, DWORD dwTrans, OVERLAPPED2* ov);
	virtual bool DispatchWrite(myServer* pServer, DWORD dwTrans, OVERLAPPED2* ov);

	OVERLAPPED2* OverlappedRecv(int type);
	OVERLAPPED2* OverlappedSend(int type, UPACKET& msg);

	virtual bool WaitReceive();
	virtual bool WaitSend(UPACKET& msg);
public:
	myNetUser();
	virtual ~myNetUser();
};

