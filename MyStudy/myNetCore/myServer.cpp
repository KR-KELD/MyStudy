#include "myServer.h"
//
//bool myServer::SendData(myNetUser & user, UPACKET & msg)
//{
//	DWORD dwSendByte;
//	int iRet;
//	user.m_wsaSendBuffer.buf = (char*)&msg;
//	user.m_wsaSendBuffer.len = msg.ph.len;
//	ZeroMemory(&user.m_ovSend, sizeof(OVERLAPPED));
//	user.m_ovSend.iType = OVERLAPPED2::MODE_SEND;
//
//	iRet = WSASend(user.m_Sock, &user.m_wsaSendBuffer, 1,
//		&dwSendByte, 0, (LPOVERLAPPED)&user.m_ovSend, NULL);
//
//	if (iRet == SOCKET_ERROR)
//	{
//		if (WSAGetLastError() != WSA_IO_PENDING)
//		{
//			return false;
//		}
//	}
//	return true;
//}

bool myServer::MakePacket(UPACKET & packet, char * msg, int iLen, uint16_t type)
{
	packet.ph.iotype = 0;
	packet.ph.len = PACKET_HEADER_SIZE + iLen;
	packet.ph.type = type;
	packet.ph.time = time(NULL);
	memcpy(&packet.msg, msg, iLen);
	return true;
}

bool myServer::Run()
{
	while (m_bStarted)
	{
#pragma region RecvPool
		{
			myLock lock(&m_RecvPacketPool);
			std::vector<myPacket>::iterator iterRecv;
			for (iterRecv = m_RecvPacketPool.m_list.begin();
				iterRecv != m_RecvPacketPool.m_list.end();
				iterRecv++)
			{
				UPACKET* packet = (UPACKET*)&iterRecv->packet;
				FunctionIterator calliter = m_fnExecutePacket.find(packet->ph.type);
				if (calliter != m_fnExecutePacket.end())
				{
					CallFuction call = calliter->second;
					(this->*call)(*iterRecv);
				}
			}
			m_RecvPacketPool.m_list.clear();
		}
		//밖으로 빼면 빈 구간에서 add가 발생시 문제가 생김
		//m_RecvPacketPool.Clear();
#pragma endregion

#pragma region SendPool
		{
			myLock lock(&m_SendPacketPool);
			std::vector<myPacket>::iterator iterSend;
			for (iterSend = m_SendPacketPool.m_list.begin();
				iterSend != m_SendPacketPool.m_list.end();
				iterSend++)
			{
				myNetUser* pUser = iterSend->pUser;
				if (pUser == nullptr) continue;
				if (pUser->WaitSend(iterSend->packet) == false)
				//if (SendData(*pUser,iterSend->packet) == false)
				{
					pUser->m_bExit = true;
					m_SessionMgr.DelUser(pUser);
				}
			}
			m_SendPacketPool.m_list.clear();
		}
#pragma endregion

		//m_SessionMgr.Lock();
		//std::map<SOCKET, myNetUser*>::iterator iterUser;
		//for (iterUser = m_SessionMgr.m_UserList.begin();
		//	iterUser != m_SessionMgr.m_UserList.end();
		//	)
		//{
		//	myNetUser* pUser = iterUser->second;
		//	bool bDelete = false;
		//	std::vector<UPACKET>::iterator senditer;
		//	//여기 추가
		//	for (senditer = pUser->m_SendPacket.begin();
		//		senditer != pUser->m_SendPacket.end();
		//		senditer++)
		//	{
		//		if (SendData(*pUser, *senditer) == false)
		//		{
		//			pUser->m_bExit = true;
		//			bDelete = true;
		//			break;
		//		}
		//	}
		//	pUser->m_SendPacket.clear();

		//	if (bDelete == true)
		//	{
		//		m_SessionMgr.CloseUser(pUser);
		//		iterUser = m_SessionMgr.m_UserList.erase(iterUser);
		//	}
		//	else
		//	{
		//		iterUser++;
		//	}
		//}
		//m_SessionMgr.UnLock();
#pragma region BroadcastPool
		{
			myLock lock(&m_SessionMgr);
			Broadcastting();
		}
#pragma endregion
	}
	return true;
}

bool myServer::Broadcastting()
{
	{
		myLock lock(&m_SendBroadcastPacketPool);
		std::map<SOCKET, myNetUser*>::iterator iterUser;
		for (iterUser = m_SessionMgr.m_UserList.begin();
			iterUser != m_SessionMgr.m_UserList.end();
			)
		{
			myNetUser* pUser = iterUser->second;
			bool bDelete = false;
			//m_SendBroadcastPacketPool.Lock();
			std::vector<myPacket>::iterator iterBroadcast;
			for (iterBroadcast = m_SendBroadcastPacketPool.m_list.begin();
				iterBroadcast != m_SendBroadcastPacketPool.m_list.end();
				iterBroadcast++)
			{
				if (pUser->WaitSend(iterBroadcast->packet) == false)
				//if (SendData(*pUser,iterBroadcast->packet) == false)
				{
					bDelete = true;
					break;
				}
			}
			//m_SendBroadcastPacketPool.Unlock();
			if (bDelete == true)
			{
				m_SessionMgr.CloseUser(pUser);
				iterUser = m_SessionMgr.m_UserList.erase(iterUser);
			}
			else
			{
				iterUser++;
			}
		}
		m_SendBroadcastPacketPool.m_list.clear();
	}
	return true;
}

bool myServer::Init()
{
	//멀티 스레드 출력 작업
	std::cout.sync_with_stdio(true);
	m_fnExecutePacket[PACKET_CHAT_MSG] = &myServer::PacketChatMsg;
	m_fnExecutePacket[PACKET_USER_POSITION] = &myServer::PacketUserPos;
	m_fnExecutePacket[PACKET_LOGIN_REQ] = &myServer::PacketLoginLeq;
	return true;
}

myServer::myServer()
{
	CreateThread();
}

myServer::~myServer()
{
}
