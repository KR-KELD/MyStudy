#pragma once
#include "myStd.h"
#include "myGameObject.h"


class myAnimation : public myComponent
{
public:
	DEFINE_COMPONENT(myAnimation, myComponent, true);
public:

public:
	myAnimation();
	virtual ~myAnimation();
};

