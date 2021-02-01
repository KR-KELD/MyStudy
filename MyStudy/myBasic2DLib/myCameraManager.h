#pragma once
#include "myStd.h"
#include "myInput.h"
class myCameraManager : public SingleTon<myCameraManager>
{
	friend class SingleTon<myCameraManager>;
private:
	myPoint			m_ptLeftTop;
	float			m_fSpeed;
	int				m_iMaxWidth;
	int				m_iMaxHeight;
public:
	bool			SetOffset(float x, float y);
	myPoint			GetOffset();
	myPoint			GetCamMousePos();
	bool			SetSpeed(float fSpeed);
	bool			SetMaxSpace(int iWidth, int iHeight);
public:
	bool			Reset();
	bool			Init();
	bool			Frame();
	bool			Render();
	bool			Release();
private:
	myCameraManager();
public:
	~myCameraManager();
};
#define g_CameraMgr myCameraManager::GetInstance()