#pragma once
#include "myCoreStd.h"
class myWindow
{
protected:
	HINSTANCE	m_hInstance;	//�ν��Ͻ� �ڵ�
	HWND		m_hWnd;			//������ �ڵ�
	DWORD		m_dwExStyle;
	DWORD		m_dwStyle;
	T_STR		m_szClassName;
	T_STR		m_szWindowName;
public:
	MSG			m_msg;			//�޽��� ����ü
	RECT		m_rtClient;		//Ŭ���̾�Ʈ ��Ʈ
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

