#pragma once
#include "myGameObject.h"
class myController : public myComponent
{
public:
	DEFINE_COMPONENT(myController, myComponent, true);
	float m_fPower = 1.0f;
public:
	virtual bool PreFrame();
	virtual bool Controller();
};

