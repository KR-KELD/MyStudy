#pragma once
#include "myCoreStd.h"
class myWindow
{
public:
	HINSTANCE		m_hInstance;	//�ν��Ͻ� �ڵ�
	HWND			m_hWnd;			//������ �ڵ�
	MSG				m_msg;			//�޽��� ����ü
	RECT			m_rtClient;		//Ŭ���̾�Ʈ ��Ʈ
public:
	bool			SetWindow(HINSTANCE hInstance);
	bool			MsgProcess();
//public:
	//virtual	void	MsgEvent(MSG msg);
public:
	myWindow();
	virtual ~myWindow();
};

