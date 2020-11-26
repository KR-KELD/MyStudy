#include "myObjectInfo.h"

void myObjectInfo::Reset()
{
	iObjType = 0;
	iTeamNum = -1;
	strName.clear();
	strObjName.clear();
	strSpriteName.clear();
	pTarget = nullptr;
	ptPos.x = 0.0f;
	ptPos.y = 0.0f;
	ptDir.x = 0.0f;
	ptDir.y = -1.0f;
	rtDest = { -1, -1, -1, -1 };
	rtSrc = { -1, -1, -1, -1 };
	fCollisionCount = 0.0f;
	iCollWidth = 0;
	iCollHeight = 0;
	iCurrentHP = 0;
	iMaxHP = 0;
	iDamage = 0;
	bDead = false;
	bHpHide = true;
	fLifeTime = -1.0f;
	fSpeed = 0.0f;
	fLookRange = 0.0f;
	fAttRange = 0.0f;
	fAttDelay = 0.0f;
	iFsmState = 0;

	bSprite = false;
	iMaxCount = 0;
	bLoop = false;
	fTmpTimer = 0.0f;
	fTimer = 0.0f;
	fAttTimer = 0.0f;
	fInterval = 0.0f;
	bIncrease = true;
	bRound = false;
	iRectIndex = 0;

}

myObjectInfo::myObjectInfo()
{
	Reset();
}

myObjectInfo::myObjectInfo(myObjectInfo * pObjInfo)
{
	Reset();
	iObjType = pObjInfo->iObjType;
	iTeamNum = pObjInfo->iTeamNum;
	strName = pObjInfo->strName;
	strObjName = pObjInfo->strObjName;
	strSpriteName = pObjInfo->strSpriteName;
	rtDest = pObjInfo->rtDest;
	rtSrc = pObjInfo->rtSrc;
	iCollWidth = pObjInfo->iCollWidth;
	iCollHeight = pObjInfo->iCollHeight;
	iMaxHP = pObjInfo->iMaxHP;
	iCurrentHP = pObjInfo->iCurrentHP;
	iDamage = pObjInfo->iDamage;
	bDead = pObjInfo->bDead;
	fLifeTime = pObjInfo->fLifeTime;
	fSpeed = pObjInfo->fSpeed;
	fLookRange = pObjInfo->fLookRange;
	fAttRange = pObjInfo->fAttRange;
	fAttDelay = pObjInfo->fAttDelay;

	bSprite = pObjInfo->bSprite;
	iMaxCount = pObjInfo->iMaxCount;
	bLoop = pObjInfo->bLoop;
	fInterval = pObjInfo->fInterval;
	bRound = pObjInfo->bRound;
}

myObjectInfo::~myObjectInfo()
{
}
