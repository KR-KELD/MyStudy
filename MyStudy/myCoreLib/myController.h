#pragma once
#include "myGameObject.h"
class myController : public myComponent
{
public:
	DEFINE_COMPONENT(myController, myComponent, true);
public:
	virtual bool Frame();
	virtual bool Controller();
};
