#pragma once
#include "myStd.h"
#include "myGameObject.h"

struct myAnimScene
{
	int iAnimStackIndex;
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
	map<wstring, myAnimScene>	m_AnimSceneMap;
	map<wstring, myAnimScene>::iterator m_AnimSceneIter;
	myAnimScene*				m_pCurrentScene;
	float						m_fTick;
public:
	bool						ChangeAnim(wstring strSceneName);
public:
	myAnimation();
	virtual ~myAnimation();
};

