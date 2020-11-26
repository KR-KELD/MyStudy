#include "myGrapicsAPI.h"
//���� DC �ʱ�ȭ
HDC		g_hScreenDC = NULL;
HDC		g_hOffScreenDC = NULL;

bool myGrapicsAPI::Init()
{
	//������ �ڵ鿡 DC ���
	m_hScreenDC = GetDC(g_hWnd);
	g_hScreenDC = m_hScreenDC;
	//offsetDC�� DC�� ����
	m_hOffScreenDC = CreateCompatibleDC(m_hScreenDC);
	//DC�� �̹����� offset��Ʈ�ʿ� ����
	m_hOffScreenBitmap = CreateCompatibleBitmap(m_hScreenDC, WINDOWSIZEX, WINDOWSIZEY);
	//offset��Ʈ���� offsetDC�� �߰�
	SelectObject(m_hOffScreenDC, m_hOffScreenBitmap);
	g_hOffScreenDC = m_hOffScreenDC;

	//������ ������ offsetDC�� �߰�
	COLORREF bkColor = BKCOLOR;
	m_hBkBrush = CreateSolidBrush(bkColor);
	SelectObject(m_hOffScreenDC, m_hBkBrush);
	return true;
}

bool myGrapicsAPI::Frame()
{
	return true;
}

bool myGrapicsAPI::Render()
{
	return true;
}

bool myGrapicsAPI::PreRender()
{
	//offsetDC�� ��Ʈ���� �ʱ�ȭ
	PatBlt(m_hOffScreenDC, 0, 0, WINDOWSIZEX, WINDOWSIZEY, PATCOPY);
	return true;
}

bool myGrapicsAPI::PostRender()
{
	//DC�� offsetDC�� �����ؼ� ����
	BitBlt(m_hScreenDC, 0, 0, WINDOWSIZEX , WINDOWSIZEY,
		m_hOffScreenDC, 0, 0, SRCCOPY);
	return true;
}

bool myGrapicsAPI::Release()
{
	DeleteObject(m_hBkBrush);
	DeleteObject(m_hOffScreenBitmap);
	ReleaseDC(g_hWnd, m_hOffScreenDC);
	ReleaseDC(g_hWnd, m_hScreenDC);
	return true;
}
