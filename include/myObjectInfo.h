#pragma once
#include "myGameObject.h"

class myObjectInfo : public myComponent
{
public:
	DEFINE_COMPONENT(myObjectInfo, null_t, true);
public:
	myObjectInfo*		pTarget;
	int					iCurrentHP;
	int					iMaxHP;
	int					iDamage;
	bool				bDead;
	float				fSpeed;
	float				fLookRange;
	float				fAttRange;
	float				fAttDelay;
	float				fLifeTime;
	int					iFsmState;
	float				fTimer;
public:
	virtual void Reset() override;
public:
	myObjectInfo();
	virtual ~myObjectInfo();
};

