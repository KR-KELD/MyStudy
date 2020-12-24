#include "myWindow.h"

//전역 핸들과 전역 클라이언트렉트 초기화
HWND		g_hWnd = 0;
HINSTANCE	g_hInstance = 0;
bool		g_bActive = false;
RECT		g_rtClient = {0, 0, 0, 0};
//프로시저
LRESULT CALLBACK WndProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		SetFocus(NULL);
		//윈도우 포커스로 잡고있는데 따로 만드는걸 추천
		g_bActive = true;
	}
	case WM_ACTIVATE:
	{
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			g_bActive = false;
		}
		else
		{
			g_bActive = true;
		}

	}break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
//윈도우 생성
bool myWindow::SetWindow(HINSTANCE hInstance)
{
	m_hInstance = hInstance;			//인스턴스 핸들
	g_hInstance = m_hInstance;
	WNDCLASSEX wc;						//윈도우 구조체
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;	//윈도우 스타일 지정
	wc.lpfnWndProc = WndProc;			//프로시저 등록
	wc.hbrBackground =					//배경색 지정
		(HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = m_hInstance;			//인스턴스 핸들 지정
	wc.lpszClassName = L"MyWindow";		//프로세스 이름 지정
	if (RegisterClassEx(&wc) == 0)
	{
		return false;
	}
	//메뉴를 제외한 클라이언트의 크기를 렉트에 담는다
	RECT rt = { 0, 0, WINDOWSIZEX, WINDOWSIZEY };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, FALSE);
	m_hWnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		L"MyWindow",						//프로세스 이름
		L"MyGame",							//프로그램 이름
		WS_OVERLAPPEDWINDOW,				//윈도우 옵션
		WINDOWSTARTX, WINDOWSTARTY,			//윈도우 시작 위치
		rt.right-rt.left, rt.bottom-rt.top,	//윈도우 크기
		nullptr, nullptr,
		m_hInstance, nullptr);
	if (m_hWnd == NULL)
		return false;
	g_hWnd = m_hWnd;						//전역 핸들에 전달
	GetClientRect(g_hWnd, &m_rtClient);		//클라이언트의 크기를 클라이언트 렉트에 담는다
	g_rtClient = m_rtClient;				//전역 클라이언트 렉트에 전달
	ShowWindow(m_hWnd, SW_SHOW);			//윈도우 띄우기
	return true;
}
//메시지큐 처리 프로세스
bool myWindow::MsgProcess()
{
	//메시지큐에 메시지가 들어있으면 가져온다
	while (PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE))
	{
		//입력받은 키를 번역한다
		TranslateMessage(&m_msg);
		//프로시저로 전달
		DispatchMessage(&m_msg);
		if (m_msg.message == WM_QUIT)
		{
			return false;
		}
		//MsgEvent(m_msg);
	}
	return true;
}

//void myWindow::MsgEvent(MSG msg)
//{
//}

myWindow::myWindow()
{
	ZeroMemory(&m_msg, sizeof(m_msg));
}

myWindow::~myWindow()
{
}
