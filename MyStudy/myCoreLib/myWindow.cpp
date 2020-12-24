#include "myWindow.h"

//���� �ڵ�� ���� Ŭ���̾�Ʈ��Ʈ �ʱ�ȭ
HWND		g_hWnd = 0;
HINSTANCE	g_hInstance = 0;
bool		g_bActive = false;
RECT		g_rtClient = {0, 0, 0, 0};
//���ν���
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
		//������ ��Ŀ���� ����ִµ� ���� ����°� ��õ
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
//������ ����
bool myWindow::SetWindow(HINSTANCE hInstance)
{
	m_hInstance = hInstance;			//�ν��Ͻ� �ڵ�
	g_hInstance = m_hInstance;
	WNDCLASSEX wc;						//������ ����ü
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;	//������ ��Ÿ�� ����
	wc.lpfnWndProc = WndProc;			//���ν��� ���
	wc.hbrBackground =					//���� ����
		(HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = m_hInstance;			//�ν��Ͻ� �ڵ� ����
	wc.lpszClassName = L"MyWindow";		//���μ��� �̸� ����
	if (RegisterClassEx(&wc) == 0)
	{
		return false;
	}
	//�޴��� ������ Ŭ���̾�Ʈ�� ũ�⸦ ��Ʈ�� ��´�
	RECT rt = { 0, 0, WINDOWSIZEX, WINDOWSIZEY };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, FALSE);
	m_hWnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		L"MyWindow",						//���μ��� �̸�
		L"MyGame",							//���α׷� �̸�
		WS_OVERLAPPEDWINDOW,				//������ �ɼ�
		WINDOWSTARTX, WINDOWSTARTY,			//������ ���� ��ġ
		rt.right-rt.left, rt.bottom-rt.top,	//������ ũ��
		nullptr, nullptr,
		m_hInstance, nullptr);
	if (m_hWnd == NULL)
		return false;
	g_hWnd = m_hWnd;						//���� �ڵ鿡 ����
	GetClientRect(g_hWnd, &m_rtClient);		//Ŭ���̾�Ʈ�� ũ�⸦ Ŭ���̾�Ʈ ��Ʈ�� ��´�
	g_rtClient = m_rtClient;				//���� Ŭ���̾�Ʈ ��Ʈ�� ����
	ShowWindow(m_hWnd, SW_SHOW);			//������ ����
	return true;
}
//�޽���ť ó�� ���μ���
bool myWindow::MsgProcess()
{
	//�޽���ť�� �޽����� ��������� �����´�
	while (PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE))
	{
		//�Է¹��� Ű�� �����Ѵ�
		TranslateMessage(&m_msg);
		//���ν����� ����
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
