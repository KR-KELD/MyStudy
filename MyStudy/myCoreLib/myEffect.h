#pragma once
#include "myObject.h"


class myEffect : public myObject
{

public:
	void		SetSprite(std::vector<RECT>& list);
	bool		Init() override;
	bool		Frame() override;
	bool		Render() override;
	bool		Release() override;
	myEffect();
	virtual ~myEffect();
};

