#include "myCameraManager.h"

bool myCameraManager::SetOffset(float x, float y)
{
	m_ptLeftTop.x = x;
	m_ptLeftTop.y = y;
	return true;
}

myPoint myCameraManager::GetOffset()
{
	return m_ptLeftTop;
}

myPoint myCameraManager::GetCamMousePos()
{
	myPoint pt;
	pt.x = (float)g_Input.GetMouse().x + m_ptLeftTop.x;
	pt.y = (float)g_Input.GetMouse().y + m_ptLeftTop.y;
	return pt;
}

bool myCameraManager::SetSpeed(float fSpeed)
{
	m_fSpeed = fSpeed;
	return true;
}

bool myCameraManager::SetMaxSpace(int iWidth, int iHeight)
{
	m_iMaxWidth = iWidth;
	m_iMaxHeight = iHeight;
	return true;
}

bool myCameraManager::Reset()
{
	return true;
}

bool myCameraManager::Init()
{
	m_ptLeftTop.x = 0.0f;
	m_ptLeftTop.x = 0.0f;
	m_fSpeed = 500.0f;
	m_iMaxWidth = 3840;
	m_iMaxHeight = 2160;
	return true;
}

bool myCameraManager::Frame()
{
	if (g_Input.GetKey(VK_RIGHT) || g_Input.GetMouse().x >= WINDOWSIZEX - 30)
	{
		m_ptLeftTop.x += m_fSpeed * g_fSecondPerFrame;
	}
	if (g_Input.GetKey(VK_LEFT) || g_Input.GetMouse().x <= 30)
	{
		m_ptLeftTop.x -= m_fSpeed * g_fSecondPerFrame;
	}
	if (g_Input.GetKey(VK_DOWN) || g_Input.GetMouse().y >= WINDOWSIZEY - 30)
	{
		m_ptLeftTop.y += m_fSpeed * g_fSecondPerFrame;
	}
	if (g_Input.GetKey(VK_UP) || g_Input.GetMouse().y <= 30)
	{
		m_ptLeftTop.y -= m_fSpeed * g_fSecondPerFrame;
	}

	if (m_ptLeftTop.x <= 0.0f) m_ptLeftTop.x = 0.0f;
	if (m_ptLeftTop.x >= m_iMaxWidth - WINDOWSIZEX)
		m_ptLeftTop.x = m_iMaxWidth - WINDOWSIZEX - EPSILON;
	if (m_ptLeftTop.y <= 0.0f) m_ptLeftTop.y = 0.0f;
	if (m_ptLeftTop.y >= m_iMaxHeight - WINDOWSIZEY)
		m_ptLeftTop.y = m_iMaxHeight - WINDOWSIZEY - EPSILON;
	return true;
}

bool myCameraManager::Render()
{
	return true;
}

bool myCameraManager::Release()
{
	return true;
}

myCameraManager::myCameraManager()
{
}

myCameraManager::~myCameraManager()
{
}
