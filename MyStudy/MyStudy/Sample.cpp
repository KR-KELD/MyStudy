#include "Sample.h"
GAMERUN;

void Sample::SendTest(const char * pData)
{
	myChatMsg chatmsg;
	memset(&chatmsg, 0, sizeof(chatmsg));

	strcpy_s(chatmsg.szName, to_wm(m_Net.m_User.szName).c_str());
	strcpy_s(chatmsg.buffer, pData);
	chatmsg.iCnt = 0;// iCount++;
	UPACKET packet;
	m_Net.MakePacket(packet, (char*)&chatmsg, sizeof(myChatMsg), PACKET_CHAT_MSG);
	m_Net.m_sendPacket.push_back(packet);
}

bool  Sample::Init()
{
	m_Net.m_User.szName = L"테스트";
	if (m_Net.InitNetwork("175.194.89.26", 10000) == false)
	{
		return false;
	}
	return true;
}

bool  Sample::Frame()
{
	if (m_Net.Frame() == false)
	{
		return true;
	}
	//함수 포인터 벡터
	std::vector<myPacket>::iterator senditer;
	for (senditer = m_Net.m_recvPacket.begin();
		senditer != m_Net.m_recvPacket.end();
		senditer++)
	{
		UPACKET* packet = (UPACKET*)&senditer->packet;
		if (packet->ph.type == PACKET_USER_POSITION)
		{
			myUnitPos* ret = (myUnitPos*)packet->msg;
			myPoint p = { ret->p[0],ret->p[1] };
			//myScene::m_pGamePlayer->SetPos(p);
		}
		if (packet->ph.type == PACKET_CHAT_MSG)
		{
			static int iCount = 0;
			iCount++;
			myChatMsg* pMsg = (myChatMsg*)&packet->msg;
			myMsg msg;
			msg.msg = to_mw(pMsg->buffer);
			msg.msg += to_wstring(iCount);

			g_Draw.Push(msg);
		}
		if (packet->ph.type == PACKET_LOGIN_ACK)
		{
			myLoginResult* ret = (myLoginResult*)packet->msg;
			if (ret->iRet == 1)
			{
				m_Net.m_bLogin = true;
			}
			else
			{
				m_Net.SendLoginData(m_Net.m_Sock, "kgca", "game");
			}
		}
	}
	m_Net.m_recvPacket.clear();

	return true;
}

bool  Sample::Render() 
{
	if (m_Net.m_bLogin && myNetwork::g_bConnect)
	{
		if (!m_Net.SendPackets())
		{
			return true;
		}

		static float fTime = 0.0f;
		fTime += g_fSecondPerFrame;
		if (fTime >= 0.01f)
		{
			SendTest("test");
			fTime -= 0.01f;
		}
	}
	return true;
}

bool  Sample::Release()
{
	m_Net.DeleteNetwork();
	return true;
}