#include "myNetUser.h"
#include "myServer.h"

// |p1------p1e:p2-------p2e:p3-------t----p3e-------|
// 1, 패킷 헤더 > dwTrans
// 2, 패킷 헤더 == dwTrans
// 3, 패킷 헤더 < dwTrans
	// 3.1, 패킷 크기 == dwTrans
	// 3.2, 패킷 크기 > dwTrans
	// 3.3, 패킷 크기 < dwTrans

//0    ~    10000
//  50byte
// --------(s)9985,9990   10;
// 임시버퍼 받는 잔여량을 저장
//clear   0    ~    10000
// 주 버퍼에 임시버퍼의 잔여량 데이터 복사
//clear   0~5: 50byte   ~    10000

bool myNetUser::DispatchRead(DWORD dwTrans, OVERLAPPED2 * ov)
{
	//다 쓴 오브랩드 구조체를 오브젝트풀에 담는다
	delete ov;
	//데이터 버퍼가 가득차면 메모리를 당겨준다
	if (m_iWritePos + dwTrans >= MAX_DATA_BUFFER_SIZE)
	{
		if (m_iReadPos > 0)
		{
			//데이터 버퍼의 시작위치에 마지막 패킷포스부터 잔여 포스만큼 복사한다
			memmove(m_szDataBuffer, &m_szDataBuffer[m_iPacketPos], m_iReadPos);
		}
		//패킷포스 초기화
		m_iPacketPos = 0;
		m_iWritePos = m_iReadPos;
	}
	// 받은 데이터 누적
	memcpy(&m_szDataBuffer[m_iWritePos], m_szRecvBuffer, dwTrans);
	m_iWritePos += dwTrans;
	m_iReadPos += dwTrans;

	//헤더 데이터 이상을 받으면
	if (m_iReadPos >= PACKET_HEADER_SIZE)
	{
		//헤더 정보를 세팅하고
		UPACKET* packet = (UPACKET*)&m_szDataBuffer[m_iPacketPos];
		// 1개의 패킷 이상의 데이터가 존재한다면

		// 패킷을 쪼개라
		do {
			myPacket tPacket;
			tPacket.pUser = this;
			//데이터 버퍼의 패킷 시작위치부터 패킷 1개 크기만큼 복사해라
			memcpy(&tPacket.packet,
				&m_szDataBuffer[m_iPacketPos],
				packet->ph.len);
			// 페킷 풀에 완성 패킷을 넣어주어야 한다.
			I_Server.m_RecvPacketPool.AddPacket(tPacket);

			//패킷을 만든만큼 리드포스를 차감해준다
			m_iReadPos -= packet->ph.len;
			//패킷포스는 다음위치로 보내준다
			m_iPacketPos += packet->ph.len;
			//더이상 패킷을 생성하지 못하면 반복문 탈출
			if (m_iReadPos < PACKET_HEADER_SIZE)
			{
				break;
			}
			//?
			//UPACKET* packet = (UPACKET*)&m_szDataBuffer[m_iPacketPos];
		} while (packet->ph.len <= m_iReadPos);

	}
	//데이터 리시브
	WaitReceive();
	return true;
}
bool myNetUser::DispatchWrite(DWORD dwTrans, OVERLAPPED2 * ov)
{
	delete ov;
	return true;
}
OVERLAPPED2 * myNetUser::OverlappedRecv(int type)
{
	OVERLAPPED2* ov = new OVERLAPPED2(type);
	ZeroMemory(m_szRecvBuffer, MAX_BUFFER_SIZE);
	m_wsaRecvBuffer.len = MAX_BUFFER_SIZE;
	m_wsaRecvBuffer.buf = m_szRecvBuffer;
	return ov;
}
OVERLAPPED2 * myNetUser::OverlappedSend(int type, UPACKET& msg)
{
	OVERLAPPED2* ov = new OVERLAPPED2(type);
	m_wsaSendBuffer.buf = (char*)&msg;
	m_wsaSendBuffer.len = msg.ph.len;
	return ov;
}
bool myNetUser::WaitReceive()
{
	//오버랩구조체 초기화
	//ZeroMemory(&m_ovRead, sizeof(OVERLAPPED));
	//m_ovRead.iType = OVERLAPPED2::MODE_RECV;
	//ZeroMemory(m_szRecvBuffer, MAX_BUFFER_SIZE);
	//m_wsaRecvBuffer.len = MAX_BUFFER_SIZE;
	//m_wsaRecvBuffer.buf = m_szRecvBuffer;
	OVERLAPPED2* ov = OverlappedRecv(OVERLAPPED2::MODE_RECV);
	//리시브버퍼 초기화
	DWORD cbTrans;	//?
	DWORD dwflags = 0;//플래그
	//리시브
	int iRet = WSARecv(m_Sock, &m_wsaRecvBuffer, 1, 
		&cbTrans, &dwflags, (LPOVERLAPPED)ov, NULL);
	if (iRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			return false;
		}
	}
	return true;
}
bool myNetUser::WaitSend(UPACKET& msg)
{
	DWORD dwSendByte;
	int iRet;
	OVERLAPPED2* ov = OverlappedSend(OVERLAPPED2::MODE_SEND, msg);
	iRet = WSASend(m_Sock, &m_wsaSendBuffer, 1,
		&dwSendByte, 0, (LPOVERLAPPED)ov, NULL);
	if (iRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			return false;
		}
	}
	return true;
}
myNetUser::myNetUser()
{
	m_bExit = false;
	m_iPacketPos = 0;
	m_iWritePos = 0;
	m_iReadPos = 0;
}
myNetUser::~myNetUser()
{

}