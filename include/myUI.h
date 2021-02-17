#pragma once
#include "myObject.h"
class myUI : public myObject
{
public:
	virtual bool		Action()override;
	virtual bool		Reset() override;
	virtual bool		Init() override;
	virtual bool		Frame() override;
	virtual bool		Render() override;
	virtual bool		Release() override;
};

