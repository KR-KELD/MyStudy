#pragma once
#include "myStd.h"
#include "myGameObject.h"
struct myTrack
{
	int iTick;
	Matrix matWorld;
};

struct myScene
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
	vector<myTrack>		m_TrackList;
	myScene				m_Scene;
public:
	myAnimation();
	virtual ~myAnimation();
};

