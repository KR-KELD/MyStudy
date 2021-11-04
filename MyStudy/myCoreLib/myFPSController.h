#pragma once
#include "myController.h"
enum class myEMoveState
{
	MOVE_STOP = 0,
	MOVE_FRONT,
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_BACK,
	MOVE_UP,
	MOVE_DOWN,
};
class myFPSController : public myController
{
public:
	DEFINE_COMPONENT(myFPSController, myController, true);
	myEMoveState m_eMoveState;
	bool m_bRun = false;
public:
	virtual bool Controller() override;
};

