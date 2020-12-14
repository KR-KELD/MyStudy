#include "myServer.h"

void myServer::PacketChatMsg(myPacket& packet)
{
	myChatMsg* pMsg = (myChatMsg*)packet.packet.msg;
	printf("\n[%s]%s:%d", pMsg->szName,
		pMsg->buffer, pMsg->iCnt);
	m_SendBroadcastPacketPool.AddPacket(packet);
}

void myServer::PacketLoginLeq(myPacket& packet)
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

void myServer::PacketUserPos(myPacket & packet)
{
	myUnitPos* pMsg = (myUnitPos*)packet.packet.msg;
	printf("\n[%10.4f:%10.4f:%10.4f",
		pMsg->p[0],
		pMsg->p[1],
		pMsg->p[2]);
	m_SendBroadcastPacketPool.AddPacket(packet);
}