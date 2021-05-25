#include "myObjectInfo.h"
DECLARE_COMPONENT(myObjectInfo)
void myObjectInfo::Reset()
{
	pTarget = nullptr;
	iCurrentHP = 0;
	iMaxHP = 0;
	iDamage = 0;
	bDead = false;
	fLifeTime = -1.0f;
	fSpeed = 0.0f;
	fLookRange = 0.0f;
	fAttRange = 0.0f;
	fAttDelay = 0.0f;
	iFsmState = 0;
	fTimer = 0.0f;
}

myObjectInfo::myObjectInfo()
{
	Reset();
}

myObjectInfo::~myObjectInfo()
{
}
