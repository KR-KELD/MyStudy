#pragma once
#include "myStd.h"
class myTimer : public SingleTon<myTimer>
{
public:
	//���� ���� �� �帥 ��ü�ð�
	float	m_fGameTimer;
	//�����Ӵ� �ɸ��� �ð�
	float	m_fSecondPerFrame;
	//�ʴ� ���ư��� ������
	int		m_iFPS;

	WCHAR	m_szBuffer[256];
	float	m_fBeforeTime;
public:

	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
	bool	Reset();
public:
	myTimer();
	virtual ~myTimer();
};
#define g_Timer myTimer::GetInstance()
