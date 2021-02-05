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
	//��ǻ�Ͱ� ���� �������� �ð��� ������ ���� ��ȯ�ϴ� �Լ�
	//1000 = 1��
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
		_stprintf_s(m_szBuffer, L"���ӽð� %8.2f�� , SPF:(%7.4f) , FPS:(%d)\n",
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

