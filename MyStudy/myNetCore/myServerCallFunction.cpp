#include "myServer.h"

void myServer::PacketChatMsg(myPacket& packet)
{
	myChatMsg* pMsg = (myChatMsg*)packet.packet.msg;
	std::cout << "[" << pMsg->szName << "] : " <<
		pMsg->buffer << " : " << pMsg->iCnt << std::endl;
	//printf("\n[%s]%s:%d", pMsg->szName,
	//	pMsg->buffer, pMsg->iCnt);
	m_SendBroadcastPacketPool.AddPacket(packet);
}

void myServer::PacketLoginLeq(myPacket& packet)
{
	myPacket userPacket;
	userPacket.pUser = packet.pUser;
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
	MakePacket(userPacket.packet, (char*)&ret, sizeof(myLoginResult),
		PACKET_LOGIN_ACK);
	if (packet.pUser != nullptr)
	{
		//packet.pUser->m_SendPacket.push_back(sendPacket);
		m_SendPacketPool.AddPacket(userPacket);
	}
}

void myServer::PacketUserPos(myPacket& packet)
{
	myUnitPos* pMsg = (myUnitPos*)packet.packet.msg;
	std::cout << pMsg->p[0] << " : " << pMsg->p[1]<< " : "
		<< pMsg->p[2] << std::endl;
	//printf("\n[%10.4f:%10.4f:%10.4f",
	//	pMsg->p[0],
	//	pMsg->p[1],
	//	pMsg->p[2]);
	m_SendBroadcastPacketPool.AddPacket(packet);
}

void myServer::PacketLogoutPlayer(myPacket & packet)
{
	m_SendBroadcastPacketPool.AddPacket(packet);
	m_SessionMgr.DelUser(packet.pUser);
}
