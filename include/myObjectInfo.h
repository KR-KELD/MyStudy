#pragma once
#include "myStd.h"

class myObjectInfo
{
public:
	int					iObjType;
	int					iTeamNum;
	wstring				strName;
	wstring				strObjName;
	wstring				strSpriteName;
	myObjectInfo*		pTarget;
	myPoint				ptPos;
	myPoint				ptDir;
	myPoint				ptDest;
	RECT				rtDest;
	RECT				rtSrc;
	RECT				rtCollider;
	float				fCollisionCount;
	int					iCollWidth;
	int					iCollHeight;
	int					iCurrentHP;
	int					iMaxHP;
	int					iDamage;
	bool				bDead;
	bool				bHpHide;
	float				fSpeed;
	float				fLookRange;
	float				fAttRange;
	float				fAttDelay;
	float				fLifeTime;
	int					iFsmState;

	bool				bSprite;
	bool				bLoop;
	float				fInterval;

	int					iMaxCount;
	float				fTmpTimer;
	float				fAttTimer;
	float				fTimer;
	bool				bIncrease;
	bool				bRound;
	int					iRectIndex;
public:
	virtual void Reset();
	myObjectInfo();
	myObjectInfo(myObjectInfo* pObjInfo);
	virtual ~myObjectInfo();
};

