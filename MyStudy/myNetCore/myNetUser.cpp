#include "myNetUser.h"
#include "mySessionMgr.h"

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
	//������ ���۰� �������� �޸𸮸� ����ش�
	if (m_iWritePos + dwTrans >= MAX_DATA_BUFFER_SIZE)
	{
		if (m_iReadPos > 0)
		{
			//������ ������ ������ġ�� ������ ��Ŷ�������� �ܿ� ������ŭ �����Ѵ�
			memmove(m_szRecvBuffer, &m_szRecvBuffer[m_iPacketPos], m_iReadPos);
		}
		//��Ŷ���� �ʱ�ȭ
		m_iPacketPos = 0;
		m_iWritePos = m_iReadPos;
	}
	if (m_iWritePos < 0)
	{
		int k = 0;
	}
	// ���� ������ ����
	memcpy(&m_szRecvBuffer[m_iWritePos], recvBuf, dwTrans);
	m_iWritePos += dwTrans;
	m_iReadPos += dwTrans;

	//��� ������ �̻��� ������
	if (m_iReadPos >= PACKET_HEADER_SIZE)
	{
		//��� ������ �����ϰ�
		UPACKET* packet = (UPACKET*)&m_szRecvBuffer[m_iPacketPos];
		// 1���� ��Ŷ �̻��� �����Ͱ� �����Ѵٸ�

		// ��Ŷ�� �ɰ���
		do {
			myPacket tPacket;
			tPacket.pUser = this;
			//������ ������ ��Ŷ ������ġ���� ��Ŷ 1�� ũ�⸸ŭ �����ض�
			memcpy(&tPacket.packet,
				&m_szRecvBuffer[m_iPacketPos],
				packet->ph.len);
			// ��Ŷ Ǯ�� �ϼ� ��Ŷ�� �־��־�� �Ѵ�.
			I_Session.AddPacket(tPacket);

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
			//UPACKET* packet = (UPACKET*)&m_szRecvBuffer[m_iPacketPos];
		} while (packet->ph.len <= m_iReadPos);

	}
	else
	{
		int k = 0;
	}
	//������ ���ú�
	WaitReceive();
	return true;
}
bool myNetUser::DispatchWrite(DWORD dwTrans, OVERLAPPED2 * ov)
{
	//ZeroMemory(recvBuf, sizeof(char) * MAX_BUFFER_SIZE);
	//m_wsaRecvBuffer.len = MAX_BUFFER_SIZE;
	//m_wsaRecvBuffer.buf = recvBuf;
	//DWORD cbTrans;
	//DWORD dwFlags = 0;
	//int iRet = WSARecv(m_Sock, &m_wsaRecvBuffer, 1,
	//	&cbTrans, &dwFlags, (OVERLAPPED*)&m_ovRead, NULL);
	//if (iRet == SOCKET_ERROR)
	//{
	//	if (WSAGetLastError() != WSA_IO_PENDING)
	//	{
	//		return false;
	//	}
	//}
	return true;
}
bool myNetUser::WaitReceive()
{
	//����������ü �ʱ�ȭ
	ZeroMemory(&m_ovRead, sizeof(OVERLAPPED));
	m_ovRead.iType = OVERLAPPED2::MODE_RECV;
	//���ú���� �ʱ�ȭ
	ZeroMemory(recvBuf, MAX_BUFFER_SIZE);
	m_wsaRecvBuffer.len = MAX_BUFFER_SIZE;
	m_wsaRecvBuffer.buf = recvBuf;
	DWORD cbTrans;	//?
	DWORD dwflags = 0;//�÷���
	//���ú�
	//���� ���� iocp�� �Ϸ� ������ ���޵Ǵ���
	int iRet = WSARecv(m_Sock, &m_wsaRecvBuffer, 1,
		&cbTrans, &dwflags, (OVERLAPPED*)&m_ovRead, NULL);
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
	iRecvSize = 0;
	iSendSize = 0;

}
myNetUser::~myNetUser()
{

}