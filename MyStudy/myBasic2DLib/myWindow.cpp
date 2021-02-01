#include "myWindow.h"

//���� �ڵ�� ���� Ŭ���̾�Ʈ��Ʈ �ʱ�ȭ
HWND		g_hWnd = 0;
HINSTANCE	g_hInstance = 0;
bool		g_bActive = false;
RECT		g_rtClient = {0, 0, 0, 0};

static bool		m_bDrag = false;
static POINT	m_ptClick;

//���ν���
LRESULT CALLBACK WndProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
	switch (message)
	{
	case WM_MBUTTONDOWN:
	{
		m_bDrag = true;
		m_ptClick.x = LOWORD(lParam);
		m_ptClick.y = HIWORD(lParam);
	}return 0;
	case WM_MOUSEMOVE:
	{
		if (m_bDrag)
		{
			RECT rtWindow;
			GetWindowRect(hWnd, &rtWindow);
			rtWindow.left -= m_ptClick.x - LOWORD(lParam);
			rtWindow.top -= m_ptClick.y - HIWORD(lParam);
#if NDEBUG
			SetWindowPos(hWnd, HWND_TOPMOST, rtWindow.left, rtWindow.top, 0, 0, SWP_NOSIZE);//HWND_TOP
#else
			SetWindowPos(hWnd, HWND_NOTOPMOST, rtWindow.left, rtWindow.top, 0, 0, SWP_NOSIZE);
#endif
		}
	}return 0;
	case WM_MBUTTONUP:
	{
		m_bDrag = false;
	}return 0;
	case WM_ACTIVATE:
	{
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			g_bActive = false;
		}
		else
		{
			::SetFocus(g_hWnd);
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
	wc.lpszClassName = m_szClassName.c_str();		//���μ��� �̸� ����
	if (RegisterClassEx(&wc) == 0)
	{
		return false;
	}
	//�޴��� ������ Ŭ���̾�Ʈ�� ũ�⸦ ��Ʈ�� ��´�
	RECT rt = { 0, 0, WINDOWSIZEX, WINDOWSIZEY };
	AdjustWindowRectEx(&rt, m_dwStyle, FALSE,m_dwExStyle);
	m_hWnd = CreateWindowEx(
		m_dwExStyle,
		m_szClassName.c_str(),				//���μ��� �̸�
		m_szWindowName.c_str(),				//���α׷� �̸�
		m_dwStyle,							//������ �ɼ�
		WINDOWSTARTX, WINDOWSTARTY,			//������ ���� ��ġ
		rt.right-rt.left, rt.bottom-rt.top,	//������ ũ��
		nullptr, nullptr,
		m_hInstance, nullptr);
	if (m_hWnd == NULL)
	{
		return false;
	}
	g_hWnd = m_hWnd;						//���� �ڵ鿡 ����
	GetClientRect(m_hWnd, &m_rtClient);		//Ŭ���̾�Ʈ�� ũ�⸦ Ŭ���̾�Ʈ ��Ʈ�� ��´�
	GetWindowRect(m_hWnd, &m_rtWindow);
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
	ZeroMemory(&m_msg, sizeof(MSG));
	m_bDrag = false;
	m_szClassName = L"MyWindow";
#if NDEBUG
	m_dwExStyle = WS_EX_TOPMOST;
	m_szWindowName = L"MyGame(Release)";
	m_dwStyle = WS_POPUPWINDOW;// WS_OVERLAPPEDWINDOW,,;
#else
	m_dwExStyle = WS_EX_APPWINDOW;
	m_szWindowName = L"MyGame(Debug)";
	m_dwStyle = WS_POPUPWINDOW;// WS_OVERLAPPEDWINDOW,,;
#endif
}

myWindow::~myWindow()
{
}
