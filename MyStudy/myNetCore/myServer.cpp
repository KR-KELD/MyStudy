#include "myServer.h"

void myServer::SendLogoutUser(myNetUser * pUser)
{
	UPACKET sendPacket;
	myLoginResult ret;
	ret.id = pUser->m_Sock;
	ret.iRet = 1;
	MakePacket(sendPacket, (char*)&ret, sizeof(myLoginResult), PACKET_LOGOUT_PLAYER);
	if (pUser != nullptr)
	{
		myPacket packet = { 0, };
		packet.pUser = pUser;
		packet.packet = sendPacket;
		m_RecvPacketPool.AddPacket(packet);
	}
	pUser->m_bExit = true;
}

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
		//������ ���� �� �������� add�� �߻��� ������ ����
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
				if (pUser->WaitSend(iterSend->packet) == false)
				{
					SendLogoutUser(pUser);
				}
			}
			m_SendPacketPool.m_list.clear();
		}
#pragma endregion
#pragma region BroadcastPool
		Broadcastting();
#pragma endregion
	}
	return true;
}

bool myServer::Broadcastting()
{
	{
		myLock lock(&m_SessionMgr);
		myLock lock(&m_SendBroadcastPacketPool);
		std::map<SOCKET, myNetUser*>::iterator iterUser;
		for (iterUser = m_SessionMgr.m_UserList.begin();
			iterUser != m_SessionMgr.m_UserList.end();
			iterUser++)
		{
			myNetUser* pUser = iterUser->second;
			std::vector<myPacket>::iterator iterBroadcast;
			for (iterBroadcast = m_SendBroadcastPacketPool.m_list.begin();
				iterBroadcast != m_SendBroadcastPacketPool.m_list.end();
				iterBroadcast++)
			{
				if (pUser->WaitSend(iterBroadcast->packet) == false)
				{
					SendLogoutUser(pUser);
				}
			}
		}
		m_SendBroadcastPacketPool.m_list.clear();
	}
	return true;
}

bool myServer::Init()
{
	//��Ƽ ������ ��� �۾�
	std::cout.sync_with_stdio(true);
	m_fnExecutePacket[PACKET_CHAT_MSG] = &myServer::PacketChatMsg;
	m_fnExecutePacket[PACKET_USER_POSITION] = &myServer::PacketUserPos;
	m_fnExecutePacket[PACKET_LOGIN_REQ] = &myServer::PacketLoginLeq;
	m_fnExecutePacket[PACKET_LOGOUT_PLAYER] = &myServer::PacketLogoutPlayer;
	return true;
}

myServer::myServer()
{
	CreateThread();
}

myServer::~myServer()
{
}