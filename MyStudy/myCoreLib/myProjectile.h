#pragma once
#include "myObject.h"

class myProjectile : public myObject
{
public:
	virtual bool		Move() override;
	virtual bool		Init() override;
	virtual bool		Frame() override;
	virtual bool		Render() override;
	virtual bool		Release() override;
	myProjectile();
	virtual ~myProjectile();
};
