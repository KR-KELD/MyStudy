#pragma once
#include "myStd.h"
#include "myGameObject.h"

struct myAnimScene
{
	//x는 애니메이션 종류, y는 시간별로 쪼갠 인덱스
	//POINT ptAnimTrackIndex;
	int iAnimTrackIndex;
	int iFirstFrame;
	int iLastFrame;
	int iFrameSpeed; // 30
	int iTickPerFrame;// 160
	int iNumMesh;
	int iDeltaTick; // 1frame
	float fDeltaTime;
	float fFirstTime;
	float fLastTime;
	myAnimScene CuttingAnimScene(int iFirstFrame, int iLastFrame)
	{
		myAnimScene scene = myAnimScene(*this);
		scene.iFirstFrame = iFirstFrame;
		scene.iLastFrame = iLastFrame;
		return scene;
	}
};

class myAnimation : public myComponent
{
public:
	DEFINE_COMPONENT(myAnimation, myComponent, true);
public:
	map<wstring, myAnimScene>	m_AnimSceneMap;
	map<wstring, myAnimScene>::iterator m_AnimSceneIter;
	myAnimScene*				m_pPrevScene;
	float						m_fPrevTick;
	bool						m_isLerp;
	float						m_fLerpTick;
	myAnimScene*				m_pCurrentScene;
	float						m_fTick;
public:
	bool						AddAnim(wstring strSceneName, myAnimScene scene);
	bool						ChangeAnim(wstring strSceneName,bool isLerp = false, float fLerpTime = 0.0f);
public:
	myAnimation();
	virtual ~myAnimation();
};

