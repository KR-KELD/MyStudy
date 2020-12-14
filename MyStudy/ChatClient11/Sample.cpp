#include "Sample.h"

void main()
{
	Sample sam;
	sam.Init();

	while (1)
	{
		sam.Timer();
		if (sam.Frame() == false)
		{
			break;
		}
		if (sam.Render() == false)
		{
			break;
		}
	}
	sam.Release();
}

void Sample::PacketChatMsg(myPacket & packet)
{
	static int iCount = 0;
	iCount++;
	myChatMsg* pMsg = (myChatMsg*)packet.packet.msg;
	string str = "[";
	str += pMsg->szName;
	str += "] : ";
	str += pMsg->buffer;
	m_listMsg.push_back(str);
}

void Sample::PacketLoginAck(myPacket & packet)
{
	myLoginResult* ret = (myLoginResult*)packet.packet.msg;
	if (ret->iRet == 1)
	{
		m_Net.m_bLogin = true;
	}
	else
	{
		m_Net.SendLoginData(m_Net.m_Sock, "kgca", "game");
	}
}

void Sample::LoginSeq()
{

	if (!m_Net.m_bLogin)
	{
		//if (m_Net.ConnectServer(ip, 10000) == false)
		//{

		//}
	}
}

void Sample::SendMsg(const char * pData)
{
	myChatMsg chatmsg;
	memset(&chatmsg, 0, sizeof(chatmsg));
	string str;
	str.assign(m_Net.m_User.strName.begin(), m_Net.m_User.strName.end());
	strcpy_s(chatmsg.szName, str.c_str());
	strcpy_s(chatmsg.buffer, pData);
	chatmsg.iCnt = 0;// iCount++;
	UPACKET packet;
	m_Net.MakePacket(packet, (char*)&chatmsg, sizeof(myChatMsg), PACKET_CHAT_MSG);
	m_Net.m_sendPacket.push_back(packet);
}

void Sample::MsgProcess()
{
	system("cls");
	if (m_listMsg.size() > 20)
	{
		m_listMsg.pop_front();
	}
	if (m_listMsg.size() < 20)
	{
		for (int i = 0; i < 20 - m_listMsg.size(); i++)
		{
			cout << endl;
		}
	}
	for (string str : m_listMsg)
	{
		cout << str << endl;
	}
	cout << m_strChat << endl;
}

void Sample::Timer()
{
	float fCurrentTime = (float)timeGetTime();
	m_fSecondPerFrame = (fCurrentTime - m_fBeforeTime) / 1000.0f;
	m_fGameTimer += m_fSecondPerFrame;
	m_fBeforeTime = fCurrentTime;
	g_fGameTimer = m_fGameTimer;
	g_fSecondPerFrame = m_fSecondPerFrame;
}

bool  Sample::Init()
{
	m_fBeforeTime = 0.0f;
	m_Net.m_User.strName = L"테스트";
	m_mapPacketFunc[PACKET_CHAT_MSG] = &Sample::PacketChatMsg;
	m_mapPacketFunc[PACKET_LOGIN_ACK] = &Sample::PacketLoginAck;
	//DWORD dwID;
	//m_hThread = CreateThread(0, 0, IPInput, (LPVOID)this,0, &dwID);
	if (m_Net.InitNetwork() == false)
	{
		return false;
	}
	system("cls");

	string ip;
	while (true)
	{
		cout << "닉네임을 입력해 주세요." << endl;
		wcin >> m_Net.m_User.strName;
		cout << "IP를 입력해 주세요." << endl;
		cin >> ip;
		if (m_Net.ConnectServer(ip, 10000) == false)
		{
		}
		break;
	}


	return true;
}

bool  Sample::Frame()
{
	if (m_Net.Frame() == false)
	{
		return false;
	}
	if (_kbhit())
	{
		int iValue = _getche();
		if (iValue == '\r')
		{
			SendMsg(m_strChat.c_str());
			m_strChat.clear();
		}
		else if (iValue == 8)
		{
			if(m_strChat.size() > 0)
				m_strChat.pop_back();
		}
		else
		{
			m_strChat += (char)iValue;
		}
	}

	static float fTime = 0.0f;
	fTime += g_fSecondPerFrame;
	if (fTime >= 1.0f)
	{
		MsgProcess();
		fTime -= 1.0f;
	}

	//함수 포인터 벡터
	std::vector<myPacket>::iterator senditer;
	for (senditer = m_Net.m_recvPacket.begin();
		senditer != m_Net.m_recvPacket.end();
		senditer++)
	{
		UPACKET* packet = (UPACKET*)&senditer->packet;
		(this->*m_mapPacketFunc[senditer->packet.ph.type])(*senditer);
	}
	m_Net.m_recvPacket.clear();

	return true;
}

bool  Sample::Render()
{
	//g_Draw.Draw(100, 100, L"IP를 입력해주세요 ");
	//for (int iKey = 0; iKey < 256; iKey++)
	//{
	//	if (g_Input.GetKey(iKey) == KEY_PUSH)
	//	{


	//	}
	//}

	//int iValue = _getche();
	//if (iValue != -1)
	//{
	//	Sample::strServerIP = (char)iValue;
	//}

	//g_Draw.Draw(100, 200, to_mw(strServerIP));
	//LoginSeq();
	if (m_Net.m_bLogin && myNetwork::g_bConnect)
	{
		if (!m_Net.SendPackets())
		{
			return true;
		}



	}
	return true;
}

bool  Sample::Release()
{
	//CloseHandle(m_hThread);
	m_Net.DeleteNetwork();
	return true;
}