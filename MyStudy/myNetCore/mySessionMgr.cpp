#include "mySessionMgr.h"

void mySessionMgr::AddPacket(myPacket & packet)
{
	WaitForSingleObject(m_hMutexPacketPool, INFINITE);
	m_PacketPool.push_back(packet);
	ReleaseMutex(m_hMutexPacketPool);
}

bool mySessionMgr::Run()
{
	while (m_bStarted)
	{
		WaitForSingleObject(m_hMutexPacketPool, INFINITE);
		std::vector<myPacket>::iterator senditer;
		for (senditer = m_PacketPool.begin();
			senditer != m_PacketPool.end();
			senditer++)
		{
			UPACKET* packet = (UPACKET*)&senditer->packet;
			if (packet->ph.type == PACKET_CHAT_MSG)
			{
				myChatMsg* pMsg = (myChatMsg*)&packet->msg;
				printf("\n[%s]%s:%d", pMsg->szName,
					pMsg->buffer, pMsg->iCnt);
			}
			if (packet->ph.type == PACKET_LOGIN_REQ)
			{
				UPACKET sendPacket;
				T_STR szID = L"kgca";
				T_STR szPS = L"game";
				myLogin* login = (myLogin*)packet->msg;
				T_STR szIDUser = to_mw(login->szID);
				T_STR szPSUser = to_mw(login->szPS);
				myLoginResult ret;
				if (szID == szIDUser && szPS == szPSUser)
				{
					ret.iRet = 1;
				}
				else
				{
					ret.iRet = 0;
				}
				/*MakePacket(sendPacket, (char*)&ret, sizeof(TLoginResult),
					PACKET_LOGIN_ACK);
				if (senditer->pUser != nullptr)
				{
					senditer->pUser->m_SendPacket.push_back(sendPacket);
				}*/
			}
		}
		m_PacketPool.clear();
		ReleaseMutex(m_hMutexPacketPool);
	}
	return true;
}

mySessionMgr::mySessionMgr()
{
	m_hMutexPacketPool = CreateMutex(NULL, FALSE, L"packetpool");
	CreateThread();
}

mySessionMgr::~mySessionMgr()
{
	CloseHandle(m_hMutexPacketPool);
}
