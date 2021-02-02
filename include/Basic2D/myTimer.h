#pragma once
#include "myStd.h"
class myTimer : public SingleTon<myTimer>
{
public:
	//게임 시작 후 흐른 전체시간
	float	m_fGameTimer;
	//프레임당 걸리는 시간
	float	m_fSecondPerFrame;
	//초당 돌아가는 프레임
	int		m_iFPS;

	WCHAR	m_szBuffer[256];
	float	m_fBeforeTime;
public:

	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
	bool	Reset();
public:
	myTimer();
	virtual ~myTimer();
};
#define g_Timer myTimer::GetInstance()
