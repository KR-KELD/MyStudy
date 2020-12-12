#pragma once
#include "myCoreStd.h"
class myWindow
{
public:
	HINSTANCE		m_hInstance;	//인스턴스 핸들
	HWND			m_hWnd;			//윈도우 핸들
	MSG				m_msg;			//메시지 구조체
	RECT			m_rtClient;		//클라이언트 렉트
public:
	bool			SetWindow(HINSTANCE hInstance);
	bool			MsgProcess();
//public:
	//virtual	void	MsgEvent(MSG msg);
public:
	myWindow();
	virtual ~myWindow();
};

