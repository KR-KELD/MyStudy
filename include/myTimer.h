#pragma once
#include "myStd.h"

class myStopwatch
{
public:
	myStopwatch() { QueryPerformanceFrequency(&m_liPerFreq); Start(); }
	void Start() { QueryPerformanceCounter(&m_liPerfStart); }

	__int64 Now()
	{    // Start 가 호출된 후 경과한 밀리초를 리턴
		LARGE_INTEGER liPerfNow;
		QueryPerformanceCounter(&liPerfNow);
		__int64 qwElapsedTime = (((liPerfNow.QuadPart - m_liPerfStart.QuadPart)));// / m_liPerFreq.QuadPart);
		return qwElapsedTime;
	}
	void Output(const TCHAR* pData)
	{
#ifdef _DEBUG
		TCHAR  strMessage[MAX_PATH] = { 0, };
		__int64 qwElapsedTime = Now();
		float Second = static_cast<float>(qwElapsedTime) / static_cast<float>(m_liPerFreq.QuadPart);
		_stprintf_s(strMessage, _T("<=====%s[%10.4f]====>\n"), pData, Second);
		OutputDebugString(strMessage);
#endif
	}
private:
	LARGE_INTEGER m_liPerFreq;    // 초당 계수
	LARGE_INTEGER m_liPerfStart;   // 시작 계수
};

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
