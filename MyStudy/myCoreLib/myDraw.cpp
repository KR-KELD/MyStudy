#include "myDraw.h"

bool myDraw::Set(HWND hWnd, int iWidth, int iHeight,
	IDXGISurface1* pSurface)
{
	// 디바이스 종속성, 디바이스 독립적
	m_iWidth = iWidth;
	m_iHeight = iHeight;

	CreateIndependentResource();
	CreateDependentResource(iWidth, iHeight, pSurface);
	return false;
}

void myDraw::ResizeDevice(int iWidth, int iHeight, IDXGISurface1 * pSurface)
{
	CreateDependentResource(iWidth, iHeight, pSurface);
}

HRESULT myDraw::CreateIndependentResource()
{
	HRESULT hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_MULTI_THREADED,
		&m_pd2dFactory);
	if (FAILED(hr))
	{
		return hr;
	}
	m_pd2dFactory->GetDesktopDpi(&m_fdpiX, &m_fdpiY);

	IDWriteFactory*		m_pd2dWrite;
	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		(IUnknown**)&m_pd2dWrite);
	if (FAILED(hr))
	{
		return hr;
	}
	hr = m_pd2dWrite->CreateTextFormat(
		L"궁서",//L"Gabriola",
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		30,
		L"ko-kr",//L"en-us",//L"ko-kr"
		&m_pTextFormat);
	if (FAILED(hr))
	{
		return hr;
	}


	return S_OK;
}

HRESULT myDraw::CreateDependentResource(int iWidth, int iHeight, IDXGISurface1 * pSurface)
{
	if (m_pd2dFactory == nullptr) return S_OK;
	D2D1_RENDER_TARGET_PROPERTIES Properties;
	Properties.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	Properties.pixelFormat.format = DXGI_FORMAT_UNKNOWN;
	Properties.pixelFormat.alphaMode =
		D2D1_ALPHA_MODE_PREMULTIPLIED;
	Properties.dpiX = m_fdpiX;
	Properties.dpiY = m_fdpiY;
	Properties.usage = D2D1_RENDER_TARGET_USAGE_NONE;
	Properties.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;

	HRESULT hr = m_pd2dFactory->CreateDxgiSurfaceRenderTarget(
		pSurface,
		&Properties,
		&m_pd2dRT);
	if (FAILED(hr))
	{
		return hr;
	}
	D2D1_COLOR_F color;
	color.r = 255.0f;
	color.g = 0.0f;
	color.b = 0.0f;
	color.a = 255.0f;

	hr = m_pd2dRT->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Red),
		&m_pBrush);
	if (FAILED(hr))
	{
		return hr;
	}
	return hr;
}

void myDraw::DeleteDependentResource()
{
	if (m_pBrush)
	{
		m_pBrush->Release();
	}
	if (m_pd2dRT)
	{
		m_pd2dRT->Release();
	}
	m_pBrush = nullptr;
	m_pd2dRT = nullptr;
}

bool myDraw::Init()
{
	return true;
}

bool myDraw::Frame()
{
	return true;
}

bool myDraw::Render()
{
	if (m_pd2dRT == nullptr) return true;
	m_pd2dRT->BeginDraw();
	{
		std::lock_guard<std::mutex> lock(m_hMutex);
		for (int iMsg = 0; iMsg < m_TextList.size(); iMsg++)
		{
			m_TextList[iMsg].rt.top = 25 + iMsg * 25;
			D2D1_MATRIX_3X2_F matWorld = D2D1::IdentityMatrix();
			m_pd2dRT->SetTransform(matWorld);
			D2D1_RECT_F layoutRect =
			{
				m_TextList[iMsg].rt.left,
				m_TextList[iMsg].rt.top,
				m_TextList[iMsg].rt.right,
				m_TextList[iMsg].rt.bottom,
			};
			D2D1::ColorF fColor = m_TextList[iMsg].dwColor;
			m_pBrush->SetColor(fColor);
			m_pd2dRT->DrawText(
				m_TextList[iMsg].msg.c_str(),
				m_TextList[iMsg].msg.size(),
				m_pTextFormat,
				&layoutRect,
				m_pBrush);
		}
	}
	m_pd2dRT->EndDraw();
	return true;
}

bool myDraw::Release()
{
	if (m_pd2dFactory == nullptr) return true;
	if (m_pBrush)m_pBrush->Release();
	if (m_pd2dFactory)m_pd2dFactory->Release();
	if (m_pd2dRT)m_pd2dRT->Release();
	if (m_pd2dWrite)m_pd2dWrite->Release();
	if (m_pTextFormat)m_pTextFormat->Release();
	return true;
}

void   myDraw::push(myMsg& msg)
{
	std::lock_guard<std::mutex> lock(m_hMutex);
	if (m_TextList.size() > 10)
	{
		m_TextList.pop_back();
	}
	m_TextList.insert(m_TextList.begin(), msg);
}
void myDraw::Draw(int x, int y, wstring msg, DWORD color)
{
	POINT p = { x, y };
	Draw(p, msg, color);
}
void myDraw::Draw(POINT pos, wstring msg, DWORD color)
{
	std::lock_guard<std::mutex> lock(m_hMutex);
	if (m_pd2dRT == nullptr) return;
	m_pd2dRT->BeginDraw();
	{
		D2D1_RECT_F layoutRect =
		{
			pos.x,pos.y, g_rtClient.right, g_rtClient.bottom
		};
		D2D1_MATRIX_3X2_F matWorld = D2D1::IdentityMatrix();
		m_pd2dRT->SetTransform(matWorld);
		D2D1::ColorF fColor = color;
		m_pBrush->SetColor(fColor);
		m_pd2dRT->DrawText(
			msg.c_str(),
			msg.size(),
			m_pTextFormat,
			&layoutRect,
			m_pBrush);
	}
	m_pd2dRT->EndDraw();
}

myDraw::myDraw()
{
	m_pd2dFactory = nullptr;
	m_pd2dRT = nullptr;
	m_pd2dWrite = nullptr;
	m_pTextFormat = nullptr;
}

myDraw::~myDraw()
{
}
//bool myDraw::Init()
//{
//	//폰트 생성
//	m_hFont = CreateFont(20, 0, 0, FW_BOLD, 0, 0, 0, 0,
//		DEFAULT_CHARSET, 0, 0, 0, 0, L"고딕");
//	//Offset Device Context에 폰트를 추가한다 
//	SelectObject(g_hOffScreenDC, m_hFont);
//	return true;
//}
////메시지 리스트에 메시지 추가
//void myDraw::Push(myMsg msg)
//{
//	if (m_TextList.size() > 10)
//	{
//		m_TextList.pop_back();
//	}
//	m_TextList.insert(m_TextList.begin(), msg);
//}
//void myDraw::Push(wstring msg, int x, int y)
//{
//	Push(myMsg(msg, { x,y,0,0 }));
//}
////함수를 통해 바로 메시지 띄우기
//void myDraw::Draw(POINT pos, wstring msg, DWORD color)
//{
//	if (g_hOffScreenDC != NULL)
//	{
//		SetBkColor(g_hOffScreenDC, RGB(255, 0,0));
//		SetTextColor(g_hOffScreenDC, color);
//		SetBkMode(g_hOffScreenDC, TRANSPARENT);
//		TextOut(g_hOffScreenDC, pos.x, pos.y, msg.c_str(), (int)(msg.size() - 1));
//	}
//}
//
//void myDraw::Draw(int x, int y, wstring msg, DWORD color)
//{
//	Draw({ x,y }, msg, color);
//}
//
//bool myDraw::Frame()
//{
//	return true;
//}
////메시지 리스트에 들어있는 메시지를 전부 화면에 그려준다
//bool myDraw::Render()
//{
//	if (g_hOffScreenDC != NULL)
//	{
//		SetBkColor(g_hOffScreenDC, RGB(255, 0, 0));
//		SetTextColor(g_hOffScreenDC, RGB(255, 255, 0));
//		SetBkMode(g_hOffScreenDC, TRANSPARENT);
//		for (int iMsg = 0; iMsg < m_TextList.size(); iMsg++)
//		{
//			DrawText(g_hOffScreenDC, m_TextList[iMsg].msg.c_str(),
//				-1, &m_TextList[iMsg].rt, DT_LEFT);
//		}
//	}
//	return true;
//}
//
//bool myDraw::Release()
//{
//	DeleteObject(m_hFont);
//	return true;
//}
