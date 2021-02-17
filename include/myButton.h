#pragma once
#include "myUI.h"
#include "myInput.h"

class myButton : public myUI
{
public:
	enum ButtonState
	{
		IDLE = 0,
		OVER,
		CLICK,
		DISABLE,
	};
	ButtonState		m_iState;
public:
	//virtual bool	MoveRect(myPoint& p)override;
	virtual bool	Action()override;
public:
	virtual bool	Reset() override;
	virtual bool	Init() override;
	virtual bool	Frame() override;
	virtual bool	Render() override;
	virtual bool	Release() override;
};

