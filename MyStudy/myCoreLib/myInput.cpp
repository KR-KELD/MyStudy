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
	ZeroMemory(&m_dwKeyState, sizeof(DWORD) * 256);
	return true;
}

bool myInput::Frame()
{
	if (g_bActive == false) return true;
	//Ŀ���� ��ġ�� �޾ƿ��� �Լ�
	GetCursorPos(&m_MousePos);
	//ȭ�� ��ǥ�� Ŭ���̾�Ʈ ��ǥ�� ��ȯ���ִ� �Լ�
	ScreenToClient(g_hWnd, &m_MousePos);
	g_KeyMap.bKeyDown = false;
	for (int iKey = 0; iKey < 256; iKey++)
	{
		//Ű�� �Է��� �񵿱�� �޾ƿ��� �Լ�(�Է��� ������ ����)
		SHORT sKey = GetAsyncKeyState(iKey);
		if (sKey & 0x8000)
		{
			if (m_dwKeyState[iKey] == KEY_FREE)
			{
				m_dwKeyState[iKey] = KEY_PUSH;
				g_KeyMap.bKeyDown = true;
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
	//���� Ű�� ���¸� �޾ƿ��� �Լ�
	SHORT dwKey = GetKeyState(VK_CAPITAL);
	//���� Ű�� Ȱ��ȭ ���¸� üũ (0x8000�� ������ �� 0x01�� ������ ����)
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

DWORD myInput::GetKey(DWORD dwKey)
{
	return m_dwKeyState[dwKey];
}

POINT& myInput::GetMouse()
{
	return m_MousePos;
}

