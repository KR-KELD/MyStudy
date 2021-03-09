#include "myInput.h"

myKeyMap g_KeyMap = { 0, };

myInput::myInput()
{
}

myInput::~myInput()
{
}

bool myInput::Init()
{
	Reset();
	m_bEnable = true;
	return true;
}

bool myInput::Frame()
{
	//if (g_bActive == false) return true;
	//커서의 위치를 받아오는 함수
	GetCursorPos(&m_MousePos);
	//화면 좌표를 클라이언트 좌표로 변환해주는 함수
	//HWND hWnd = WindowFromPoint(m_MousePos);
	ScreenToClient(g_hWnd, &m_MousePos);
	//if (hWnd != g_hWnd) return true;

	if (m_bEnable == false) return true;
	for (int iKey = 0; iKey < 256; iKey++)
	{
		//키의 입력을 비동기로 받아오는 함수(입력한 시점을 감지)
		SHORT sKey = GetAsyncKeyState(iKey);
		if (sKey & 0x8000)
		{
			if (m_dwKeyState[iKey] == KEY_FREE)
			{
				m_dwKeyState[iKey] = KEY_PUSH;
			}
			else
			{
				m_dwKeyState[iKey] = KEY_HOLD;
			}
		}
		else
		{
			if (m_dwKeyState[iKey] == KEY_PUSH ||
				m_dwKeyState[iKey] == KEY_HOLD)
			{
				m_dwKeyState[iKey] = KEY_UP;
			}
			else
			{
				m_dwKeyState[iKey] = KEY_FREE;
			}
		}
	}
	g_KeyMap.bCapsLockActive = false;
	//현재 키의 상태를 받아오는 함수
	SHORT dwKey = GetKeyState(VK_CAPITAL);
	//현재 키의 활성화 상태를 체크 (0x8000은 눌렸을 때 0x01은 눌려진 상태)
	if (dwKey & 0x01)
		g_KeyMap.bCapsLockActive = true;
	return true;
}

bool myInput::Render()
{
	return true;
}

bool myInput::Release()
{
	return true;
}

bool myInput::Reset()
{
	ZeroMemory(&m_dwKeyState, sizeof(DWORD) * 256);
	ZeroMemory(&g_KeyMap, sizeof(myKeyMap));
	return true;
}

DWORD myInput::GetKey(DWORD dwKey)
{
	return m_dwKeyState[dwKey];
}

POINT& myInput::GetMouse()
{
	return m_MousePos;
}

void myInput::SetEnable(bool bEnable)
{
	m_bEnable = bEnable;
	if (m_bEnable == false)
	{
		ZeroMemory(&m_dwKeyState, sizeof(DWORD) * 256);
		ZeroMemory(&g_KeyMap, sizeof(myKeyMap));
	}
}

bool myInput::GetEnable()
{
	return m_bEnable;
}

