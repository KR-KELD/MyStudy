#include "myNetUser.h"
#include "myServer.h"

// |p1------p1e:p2-------p2e:p3-------t----p3e-------|
// 1, ��Ŷ ��� > dwTrans
// 2, ��Ŷ ��� == dwTrans
// 3, ��Ŷ ��� < dwTrans
	// 3.1, ��Ŷ ũ�� == dwTrans
	// 3.2, ��Ŷ ũ�� > dwTrans
	// 3.3, ��Ŷ ũ�� < dwTrans

//0    ~    10000
//  50byte
// --------(s)9985,9990   10;
// �ӽù��� �޴� �ܿ����� ����
//clear   0    ~    10000
// �� ���ۿ� �ӽù����� �ܿ��� ������ ����
//clear   0~5: 50byte   ~    10000

bool myNetUser::DispatchRead(DWORD dwTrans, OVERLAPPED2 * ov)
{
	//�� �� ���귦�� ����ü�� ������ƮǮ�� ��´�
	delete ov;
	//������ ���۰� �������� �޸𸮸� ����ش�
	if (m_iWritePos + dwTrans >= MAX_DATA_BUFFER_SIZE)
	{
		if (m_iReadPos > 0)
		{
			//������ ������ ������ġ�� ������ ��Ŷ�������� �ܿ� ������ŭ �����Ѵ�
			memmove(m_szDataBuffer, &m_szDataBuffer[m_iPacketPos], m_iReadPos);
		}
		//��Ŷ���� �ʱ�ȭ
		m_iPacketPos = 0;
		m_iWritePos = m_iReadPos;
	}
	// ���� ������ ����
	memcpy(&m_szDataBuffer[m_iWritePos], m_szRecvBuffer, dwTrans);
	m_iWritePos += dwTrans;
	m_iReadPos += dwTrans;

	//��� ������ �̻��� ������
	if (m_iReadPos >= PACKET_HEADER_SIZE)
	{
		//��� ������ �����ϰ�
		UPACKET* packet = (UPACKET*)&m_szDataBuffer[m_iPacketPos];
		// 1���� ��Ŷ �̻��� �����Ͱ� �����Ѵٸ�

		// ��Ŷ�� �ɰ���
		do {
			myPacket tPacket;
			tPacket.pUser = this;
			//������ ������ ��Ŷ ������ġ���� ��Ŷ 1�� ũ�⸸ŭ �����ض�
			memcpy(&tPacket.packet,
				&m_szDataBuffer[m_iPacketPos],
				packet->ph.len);
			// ��Ŷ Ǯ�� �ϼ� ��Ŷ�� �־��־�� �Ѵ�.
			I_Server.m_RecvPacketPool.AddPacket(tPacket);

			//��Ŷ�� ���縸ŭ ���������� �������ش�
			m_iReadPos -= packet->ph.len;
			//��Ŷ������ ������ġ�� �����ش�
			m_iPacketPos += packet->ph.len;
			//���̻� ��Ŷ�� �������� ���ϸ� �ݺ��� Ż��
			if (m_iReadPos < PACKET_HEADER_SIZE)
			{
				break;
			}
			//?
			//UPACKET* packet = (UPACKET*)&m_szDataBuffer[m_iPacketPos];
		} while (packet->ph.len <= m_iReadPos);

	}
	//������ ���ú�
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
	//����������ü �ʱ�ȭ
	//ZeroMemory(&m_ovRead, sizeof(OVERLAPPED));
	//m_ovRead.iType = OVERLAPPED2::MODE_RECV;
	//ZeroMemory(m_szRecvBuffer, MAX_BUFFER_SIZE);
	//m_wsaRecvBuffer.len = MAX_BUFFER_SIZE;
	//m_wsaRecvBuffer.buf = m_szRecvBuffer;
	OVERLAPPED2* ov = OverlappedRecv(OVERLAPPED2::MODE_RECV);
	//���ú���� �ʱ�ȭ
	DWORD cbTrans;	//?
	DWORD dwflags = 0;//�÷���
	//���ú�
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