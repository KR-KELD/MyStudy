#include "myDraw.h"

bool myDraw::Init()
{
	//폰트 생성
	m_hFont = CreateFont(20, 0, 0, FW_BOLD, 0, 0, 0, 0,
		DEFAULT_CHARSET, 0, 0, 0, 0, L"고딕");
	//Offset Device Context에 폰트를 추가한다 
	SelectObject(g_hOffScreenDC, m_hFont);
	return true;
}
//메시지 리스트에 메시지 추가
void myDraw::Push(myMsg msg)
{
	if (m_TextList.size() > 10)
	{
		m_TextList.pop_back();
	}
	m_TextList.insert(m_TextList.begin(), msg);
}
void myDraw::Push(wstring msg, int x, int y)
{
	Push(myMsg(msg, { x,y,0,0 }));
}
//함수를 통해 바로 메시지 띄우기
void myDraw::Draw(POINT pos, wstring msg, DWORD color)
{
	if (g_hOffScreenDC != NULL)
	{
		SetBkColor(g_hOffScreenDC, RGB(255, 0,0));
		SetTextColor(g_hOffScreenDC, color);
		SetBkMode(g_hOffScreenDC, TRANSPARENT);
		TextOut(g_hOffScreenDC, pos.x, pos.y, msg.c_str(), (int)(msg.size() - 1));
	}
}

void myDraw::Draw(int x, int y, wstring msg, DWORD color)
{
	Draw({ x,y }, msg, color);
}

bool myDraw::Frame()
{
	return true;
}
//메시지 리스트에 들어있는 메시지를 전부 화면에 그려준다
bool myDraw::Render()
{
	if (g_hOffScreenDC != NULL)
	{
		SetBkColor(g_hOffScreenDC, RGB(255, 0, 0));
		SetTextColor(g_hOffScreenDC, RGB(255, 255, 0));
		SetBkMode(g_hOffScreenDC, TRANSPARENT);
		for (int iMsg = 0; iMsg < m_TextList.size(); iMsg++)
		{
			DrawText(g_hOffScreenDC, m_TextList[iMsg].msg.c_str(),
				-1, &m_TextList[iMsg].rt, DT_LEFT);
		}
	}
	return true;
}

bool myDraw::Release()
{
	DeleteObject(m_hFont);
	return true;
}

myDraw::myDraw()
{
}

myDraw::~myDraw()
{
}
