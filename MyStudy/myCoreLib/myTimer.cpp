#include "myTimer.h"

float g_fGameTimer = 0.0f;
float g_fSecondPerFrame = 0.0f;

myTimer::myTimer()
{
}

myTimer::~myTimer()
{
}

bool myTimer::Init()
{
	m_fGameTimer = 0.0f;
	m_fSecondPerFrame = 0.0f;
	m_iFPS = 0;
	ZeroMemory(m_szBuffer, sizeof(WCHAR) * 256);
	m_fBeforeTime = (float)timeGetTime();
	return true;
}

bool myTimer::Frame()
{
	//컴퓨터가 켜진 시점부터 시간을 누적한 값을 반환하는 함수
	//1000 = 1초
	float fCurrentTime = (float)timeGetTime();
	m_fSecondPerFrame = (fCurrentTime - m_fBeforeTime) / 1000.0f;
	m_fGameTimer += m_fSecondPerFrame;
	m_fBeforeTime = fCurrentTime;

	g_fGameTimer = m_fGameTimer;
	g_fSecondPerFrame = m_fSecondPerFrame;
	return true;
}

bool myTimer::Render()
{
	static float fTimer = 0.0f;
	fTimer += m_fSecondPerFrame;
	if (fTimer >= 1.0f)
	{
		ZeroMemory(m_szBuffer, sizeof(WCHAR) * 256);
		_stprintf_s(m_szBuffer, L"게임시간 %8.2f초 , SPF:(%7.4f) , FPS:(%d)\n",
			m_fGameTimer, m_fSecondPerFrame, m_iFPS);
		//OutputDebugString(m_szBuffer);
		fTimer -= 1.0f;
		m_iFPS = 0;
	}
	m_iFPS++;
	return true;
}

bool myTimer::Release()
{
	return true;
}


bool myTimer::Reset()
{
	m_fGameTimer = 0.0f;
	m_fSecondPerFrame = 0.0f;
	m_iFPS = 0;
	return true;
}

