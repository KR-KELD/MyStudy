#include "myGrapicsAPI.h"
//전역 DC 초기화
HDC		g_hScreenDC = NULL;
HDC		g_hOffScreenDC = NULL;

bool myGrapicsAPI::Init()
{
	//윈도우 핸들에 DC 등록
	m_hScreenDC = GetDC(g_hWnd);
	g_hScreenDC = m_hScreenDC;
	//offsetDC에 DC를 복사
	m_hOffScreenDC = CreateCompatibleDC(m_hScreenDC);
	//DC의 이미지를 offset비트맵에 복사
	m_hOffScreenBitmap = CreateCompatibleBitmap(m_hScreenDC, WINDOWSIZEX, WINDOWSIZEY);
	//offset비트맵을 offsetDC에 추가
	SelectObject(m_hOffScreenDC, m_hOffScreenBitmap);
	g_hOffScreenDC = m_hOffScreenDC;

	//배경색을 생성후 offsetDC에 추가
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
	//offsetDC의 비트맵을 초기화
	PatBlt(m_hOffScreenDC, 0, 0, WINDOWSIZEX, WINDOWSIZEY, PATCOPY);
	return true;
}

bool myGrapicsAPI::PostRender()
{
	//DC에 offsetDC를 복사해서 전달
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
