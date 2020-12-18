#include "Sample.h"
GAMERUN;
HWND	g_hEdit;
HWND	g_hButton;
RECT	g_rtEdit;
RECT	g_rtButton;


void Sample::PacketChatMsg(myPacket & packet)
{
	static int iCount = 0;
	iCount++;
	myChatMsg* pMsg = (myChatMsg*)packet.packet.msg;
	myMsg msg;
	msg.msg = to_mw(pMsg->buffer);
	msg.msg += to_wstring(iCount);
	msg.rt.top = 100;
	msg.rt.left = 100;
	g_Draw.Push(msg);
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

void Sample::SendTest(const char * pData)
{
	myChatMsg chatmsg;
	memset(&chatmsg, 0, sizeof(chatmsg));

	strcpy_s(chatmsg.szName, to_wm(m_Net.m_User.strName).c_str());
	strcpy_s(chatmsg.buffer, pData);
	chatmsg.iCnt = 0;// iCount++;
	UPACKET packet;
	m_Net.MakePacket(packet, (char*)&chatmsg, sizeof(myChatMsg), PACKET_CHAT_MSG);
	m_Net.m_sendPacket.push_back(packet);
}

DWORD WINAPI IPInput(LPVOID arg)
{
	char buf[500] = { 0, };
	int iMsgLength = 0;
	while (1)
	{
		if (g_KeyMap.bKeyDown)
		{
			int iValue = _getche();
			if (iValue == '\r')
			{
				iMsgLength = 0;
			}
			else
			{
				buf[iMsgLength++] = iValue;
				Sample::strServerIP = buf;
			}
		}
	}
	return 0;
}

bool  Sample::Init()
{
	g_rtEdit = { 0,g_rtClient.bottom - 30, g_rtClient.right - 100, 30 };
	g_rtButton = { g_rtEdit.right + 10, g_rtEdit.top, 50, 20 };
	WNDCLASS WndClass;
	GetClassInfo(NULL, L"edit", &WndClass);
	WndClass.style = CS_OWNDC;
	WndClass.lpszClassName = L"myEdit";
	RegisterClass(&WndClass);
	DWORD style = WS_CHILD | WS_VISIBLE;
	g_hEdit = CreateWindow(L"myEdit", NULL, style,
		g_rtEdit.left, g_rtClient.top, g_rtClient.right,
		g_rtClient.bottom, g_hWnd, (HMENU)100, g_hInstance, 0);
	SendMessage(g_hEdit, WM_SETFONT, (WPARAM)g_Draw.m_hFont,NULL);
	g_hButton = CreateWindow(L"button",L"Send", style,
		g_rtEdit.left, g_rtClient.top, g_rtClient.right,
		g_rtClient.bottom, g_hWnd, (HMENU)200, g_hInstance, NULL);
	//g_OldButtonProc = SetWindowLongPtr(g_hButton, GWLP_WNDPROC, (LONG_PTR)ButtonProc);

	m_Net.m_User.strName = L"테스트";
	m_vecPacketFunc[PACKET_CHAT_MSG] = &Sample::PacketChatMsg;
	m_vecPacketFunc[PACKET_LOGIN_ACK] = &Sample::PacketLoginAck;
	//DWORD dwID;
	//m_hThread = CreateThread(0, 0, IPInput, (LPVOID)this,0, &dwID);
	if (m_Net.InitNetwork() == false)
	{
		return true;
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
		(this->*m_vecPacketFunc[senditer->packet.ph.type])(*senditer);
		//if (packet->ph.type == PACKET_CHAT_MSG)
		//{

		//}
		//if (packet->ph.type == PACKET_LOGIN_ACK)
		//{

		//}
	}
	m_Net.m_recvPacket.clear();

	return true;
}

bool  Sample::Render() 
{
	g_Draw.Draw(100, 100, L"IP를 입력해주세요 ");
	for (int iKey = 0; iKey < 256; iKey++)
	{
		if (g_Input.GetKey(iKey) == KEY_PUSH)
		{
			
			
		}
	}

	int iValue = _getche();
	if (iValue != -1)
	{
		Sample::strServerIP = (char)iValue;
	}




	g_Draw.Draw(100, 200, to_mw(strServerIP));
	LoginSeq();
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
	//CloseHandle(m_hThread);
	m_Net.DeleteNetwork();
	return true;
}