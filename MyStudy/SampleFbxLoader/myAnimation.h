#pragma once
#include "myStd.h"
#include "myGameObject.h"

struct myAnimScene
{
	int iFirstFrame;
	int iLastFrame;
	int iFrameSpeed; // 30
	int iTickPerFrame;// 160
	int iNumMesh;
	int iDeltaTick; // 1frame
	float fDeltaTime;
	float fFirstTime;
	float fLastTime;
};

class myAnimation : public myComponent
{
public:
	DEFINE_COMPONENT(myAnimation, myComponent, true);
public:
	myAnimScene			m_AnimScene;
	int					m_iTick = 0;
public:
	myAnimation();
	virtual ~myAnimation();
};

