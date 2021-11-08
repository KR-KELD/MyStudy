#include "myTimer.h"

float g_fGameTimer = 0.0f;
float g_fSecondPerFrame = 0.0f;

myTimer::myTimer()
{
	m_fGameTimer = 0.0f;
	m_fSecondPerFrame = 0.0f;
	m_iFPS = 0;
	m_iTmpCounter = 0;
	QueryPerformanceFrequency(&m_Frequency);
}

myTimer::~myTimer()
{
}

bool myTimer::Init()
{
	QueryPerformanceCounter(&m_Frame);
	return true;
}

bool myTimer::Frame()
{
	QueryPerformanceCounter(&m_Current);
	m_fSecondPerFrame =
		static_cast<float>(m_Current.QuadPart - m_Frame.QuadPart)
		/ static_cast<float>(m_Frequency.QuadPart);
	g_fSecondPerFrame = m_fSecondPerFrame;

	static float fpsTime = 0.0f;
	m_iTmpCounter++;
	fpsTime += m_fSecondPerFrame;
	if (fpsTime >= 1.0f)
	{
		m_iFPS = m_iTmpCounter;
		m_iTmpCounter = 0;
		fpsTime -= 1.0f;
	}

	m_fGameTimer += m_fSecondPerFrame;
	g_fGameTimer = m_fGameTimer;
	m_Frame = m_Current;
	return true;

}

bool myTimer::Render()
{
	_stprintf_s(m_szBuffer, L"게임시간 %8.2f초 , SPF:(%7.4f) , FPS:(%d)\n",
		m_fGameTimer, m_fSecondPerFrame, m_iFPS);
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
	m_iTmpCounter = 0;
	return true;
}

