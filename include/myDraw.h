#pragma once
#include "myStd.h"
#include <mutex>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")
struct myMsg
{
	RECT	rt;
	wstring	msg;
	DWORD   dwColor;
	myMsg(wstring _msg, RECT _rt, DWORD _dwColor)
	{
		msg = _msg;
		rt = _rt;
		dwColor = _dwColor;
	}
	myMsg()
	{

	}
};
class myDraw : public SingleTon<myDraw>
{
public:
	ID2D1Factory*		m_pd2dFactory;
	ID2D1RenderTarget*  m_pd2dRT;
	IDWriteFactory*		m_pd2dWrite;
	IDWriteTextFormat*  m_pTextFormat;
	ID2D1SolidColorBrush*	m_pBrush = nullptr;
	UINT				m_iWidth;
	UINT				m_iHeight;
	float				m_fdpiX;
	float				m_fdpiY;
	std::vector<myMsg>   m_TextList;
public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();
	virtual bool Set(HWND hWnd,
		int iWidth, int iHeight,
		IDXGISurface1* pSurface);
	void ResizeDevice(int iWidth, int iHeight,
		IDXGISurface1* pSurface);
	HRESULT CreateIndependentResource();
	HRESULT CreateDependentResource(int iWidth, int iHeight,
		IDXGISurface1* pSurface);
	void DeleteDependentResource();
public:
	std::mutex      m_hMutex;
	virtual void   push(myMsg& msg);
	virtual void   Draw(POINT pos, wstring msg, DWORD color = RGB(255, 0, 0));
	virtual void   Draw(int x, int y, wstring msg, DWORD color = RGB(255, 0, 0));
public:
	myDraw();
	virtual ~myDraw();
};

//class myDraw : public SingleTon<myDraw>
//{
//public:
//	HFONT			m_hFont;
//	vector<myMsg>	m_TextList;
//public:
//	void	Push(myMsg msg);
//	void	Push(wstring msg, int x = 0, int y = 0);
//	void	Draw(POINT pos, wstring msg, DWORD color = RGB(255,0,0));
//	void	Draw(int x, int y, wstring msg, DWORD color = RGB(255, 0, 0));
//public:
//	bool	Init();
//	bool	Frame();
//	bool	Render();
//	bool	Release();
//public:
//	myDraw();
//	virtual ~myDraw();
//};
#define g_Draw myDraw::GetInstance()
