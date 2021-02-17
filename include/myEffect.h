#pragma once
#include "myObject.h"


class myEffect : public myObject
{

public:
	bool		Init() override;
	bool		Frame() override;
	bool		Render() override;
	bool		Release() override;
	myEffect();
	virtual ~myEffect();
};

