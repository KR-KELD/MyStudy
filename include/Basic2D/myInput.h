#pragma once
#include "myStd.h"

enum KeyState
{
	KEY_FREE = 0,
	KEY_PUSH,
	KEY_UP,
	KEY_HOLD
};

class myInput : public SingleTon<myInput>
{
	friend class SingleTon<myInput>;
private:
	DWORD		m_dwKeyState[256];
	POINT		m_MousePos;
public:
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
public:
	DWORD		GetKey(DWORD dwKey);
	POINT&		GetMouse();
private:
	myInput();
public:
	~myInput();

};
#define g_Input myInput::GetInstance()

