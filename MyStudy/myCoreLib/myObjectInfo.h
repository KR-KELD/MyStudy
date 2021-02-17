#pragma once
#include "myComponent.h"

class myObjectInfo : public myComponent
{
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

