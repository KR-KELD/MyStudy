#include "mySessionMgr.h"

//패킷생성
bool mySessionMgr::MakePacket(UPACKET& packet,
	char* msg, int iLen, uint16_t type)
{
	packet.ph.iotype = 0;
	packet.ph.len = PACKET_HEADER_SIZE + iLen;
	packet.ph.type = type;
	packet.ph.time = time(NULL);
	memcpy(&packet.msg, msg, iLen);
	return true;
}

void mySessionMgr::PacketChatMsg(myPacket& packet)
{
	myChatMsg* pMsg = (myChatMsg*)packet.packet.msg;
	printf("\n[%s]%s:%d", pMsg->szName,
		pMsg->buffer, pMsg->iCnt);
	m_BroadcasttingPacketPool.push_back(packet.packet);
}

void mySessionMgr::PacketLoginLeq(myPacket& packet)
{
	UPACKET sendPacket;
	T_STR szID = L"kgca";
	T_STR szPS = L"game";
	myLogin* login = (myLogin*)packet.packet.msg;
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
	MakePacket(sendPacket, (char*)&ret, sizeof(myLoginResult),
		PACKET_LOGIN_ACK);
	if (packet.pUser != nullptr)
	{
		packet.pUser->m_SendPacket.push_back(sendPacket);
	}
}

void mySessionMgr::PacketUserPos(myPacket & packet)
{
	myUnitPos* pMsg = (myUnitPos*)packet.packet.msg;
	printf("\n[%10.4f:%10.4f:%10.4f",
		pMsg->p[0],
		pMsg->p[1],
		pMsg->p[2]);
	m_BroadcasttingPacketPool.push_back(packet.packet);
}

bool mySessionMgr::SendData(myNetUser& user, UPACKET& msg)
{
	DWORD dwSendByte;
	int iRet;
	user.m_wsaSendBuffer.buf = (char*)&msg;
	user.m_wsaSendBuffer.len = msg.ph.len;
	ZeroMemory(&user.m_ovSend, sizeof(OVERLAPPED));
	user.m_ovSend.iType = OVERLAPPED2::MODE_SEND;

	iRet = WSASend(user.m_Sock, &user.m_wsaSendBuffer, 1,
		&dwSendByte, 0, (LPOVERLAPPED)&user.m_ovSend, NULL);

	if (iRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			return false;
		}
	}
	return true;
}

void mySessionMgr::AddUser(myNetUser * pUser)
{
	{
		myLock lock(this);
		this->m_UserList.push_back(pUser);
	}
}

bool mySessionMgr::DelUser(myNetUser* pUser)
{
	{
		myLock lock(this);
		printf("\n해제->%s:%d",
			inet_ntoa(pUser->m_SockAddr.sin_addr),
			ntohs(pUser->m_SockAddr.sin_port));

		CloseUser(pUser);
		std::list<myNetUser*>::iterator iter =
			std::find(m_UserList.begin(), m_UserList.end(), pUser);
		if (iter != m_UserList.end())
		{
			m_UserList.erase(iter);
		}

	}
	return true;
}

bool mySessionMgr::CloseUser(myNetUser* pUser)
{
	if (pUser == nullptr) return true;
	shutdown(pUser->m_Sock, SD_SEND);
	closesocket(pUser->m_Sock);
	delete pUser;
	pUser = nullptr;
	return true;
}

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
			//UPACKET* packet = (UPACKET*)&senditer->packet;
			(this->*m_vecPacketFunc[senditer->packet.ph.type])(*senditer);

			//if (packet->ph.type == PACKET_CHAT_MSG)
			//{

			//}
			//if (packet->ph.type == PACKET_LOGIN_REQ)
			//{

			//}
		}
		m_PacketPool.clear();
		ReleaseMutex(m_hMutexPacketPool);


		{
			myLock lock(this);
			std::list<myNetUser*>::iterator iter;
			for (iter = m_UserList.begin();
				iter != m_UserList.end();
				)
			{
				myNetUser* pUser = *iter;
				bool bDelete = false;
				std::vector<UPACKET>::iterator senditer;
				for (senditer = pUser->m_SendPacket.begin();
					senditer != pUser->m_SendPacket.end();
					senditer++)
				{
					if (SendData(*pUser, *senditer) == false)
					{
						pUser->m_bExit = true;
						bDelete = true;
						break;
					}
				}
				pUser->m_SendPacket.clear();

				if (bDelete == true)
				{
					CloseUser(pUser);
					iter = m_UserList.erase(iter);
				}
				else
				{
					iter++;
				}
			}
		}


		{
			myLock lock(this);
			Broadcastting();
		}

	}
	return true;
}

bool mySessionMgr::Broadcastting()
{
	std::list<myNetUser*>::iterator iter;
	for (iter = m_UserList.begin();
		iter != m_UserList.end();
		)
	{
		myNetUser* pUser = *iter;
		bool bDelete = false;
		std::vector<UPACKET>::iterator senditer;

		WaitForSingleObject(m_hMutexPacketPool, INFINITE);
		for (senditer = m_BroadcasttingPacketPool.begin();
			senditer != m_BroadcasttingPacketPool.end();
			senditer++)
		{
			if (SendData(*pUser, *senditer) == false)
			{
				bDelete == true;
				break;
			}
		}
		if (bDelete == true)
		{
			CloseUser(pUser);
			iter = m_UserList.erase(iter);
		}
		else
		{
			iter++;
		}
		ReleaseMutex(m_hMutexPacketPool);
	}

	WaitForSingleObject(m_hMutexPacketPool, INFINITE);
	m_BroadcasttingPacketPool.clear();
	ReleaseMutex(m_hMutexPacketPool);
	return true;
}


mySessionMgr::mySessionMgr()
{
	m_hMutexPacketPool = CreateMutex(NULL, FALSE, L"packetpool");
	CreateThread();
	m_vecPacketFunc[PACKET_CHAT_MSG] = &mySessionMgr::PacketChatMsg;
	m_vecPacketFunc[PACKET_LOGIN_REQ] = &mySessionMgr::PacketLoginLeq;
	m_vecPacketFunc[PACKET_USER_POSITION] = &mySessionMgr::PacketUserPos;
}

mySessionMgr::~mySessionMgr()
{
	CloseHandle(m_hMutexPacketPool);
}
