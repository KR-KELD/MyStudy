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
		//밖으로 빼면 빈 구간에서 add가 발생시 문제가 생김
		//m_RecvPacketPool.Clear();
#pragma endregion RecvPool
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
#pragma endregion SendPool
#pragma region BroadcastPool
		myLock lock(&m_SessionMgr);
		{
			Broadcastting();
		}
#pragma endregion BroadcastPool
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
	m_IOCP = new myIOCP;
	m_IOCP->GetServer(this);
	m_Acceptor = new myAcceptor;
	m_Acceptor->GetServer(this);
	m_Acceptor->InitNetwork("", 10000);
	m_Acceptor->CreateThread();
	CreateThread();
	
	m_fnExecutePacket[PACKET_CHAT_MSG] = &myServer::PacketChatMsg;
	m_fnExecutePacket[PACKET_USER_POSITION] = &myServer::PacketUserPos;
	m_fnExecutePacket[PACKET_LOGIN_REQ] = &myServer::PacketLoginLeq;
	m_fnExecutePacket[PACKET_LOGOUT_PLAYER] = &myServer::PacketLogoutPlayer;
	return true;
}

bool myServer::Release()
{
	m_IOCP->Release();
	m_Acceptor->DeleteNetwork();
	delete m_IOCP;
	delete m_Acceptor;
	return true;
}

myServer::myServer()
{
	//멀티 스레드 출력 작업
	std::cout.sync_with_stdio(true);
}

myServer::~myServer()
{

}
