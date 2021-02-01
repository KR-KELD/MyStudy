#pragma once
#include "myCoreStd.h"
class myWindow
{
protected:
	HINSTANCE	m_hInstance;	//인스턴스 핸들
	HWND		m_hWnd;			//윈도우 핸들
	DWORD		m_dwExStyle;
	DWORD		m_dwStyle;
	T_STR		m_szClassName;
	T_STR		m_szWindowName;
public:
	MSG			m_msg;			//메시지 구조체
	RECT		m_rtClient;		//클라이언트 렉트
	RECT		m_rtWindow;
	bool		m_bDrag;
	POINT		m_ptClick;
public:
	bool		SetWindow(HINSTANCE hInstance);
	bool		MsgProcess();
//public:
	//virtual	void	MsgEvent(MSG msg);
public:
	myWindow();
	virtual ~myWindow();
};

