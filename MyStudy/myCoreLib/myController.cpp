#include "myController.h"
#include "myInput.h"
DECLARE_COMPONENT(myController);
bool myController::PreFrame()
{
	Controller();
	return true;
}

bool myController::Controller()
{
	if (g_Input.GetKey('W') == KEY_HOLD)
	{
		m_pTransform->FrontMovement(1.0f);
	}
	if (g_Input.GetKey('S') == KEY_HOLD)
	{
		m_pTransform->FrontMovement(-1.0f);
	}
	if (g_Input.GetKey('A') == KEY_HOLD)
	{
		m_pTransform->RightMovement(-1.0f);
	}
	if (g_Input.GetKey('D') == KEY_HOLD)
	{
		m_pTransform->RightMovement(1.0f);
	}
	if (g_Input.GetKey('Q') == KEY_HOLD)
	{
		m_pTransform->UpMovement(1.0f);
	}
	if (g_Input.GetKey('E') == KEY_HOLD)
	{
		m_pTransform->UpMovement(-1.0f);
	}
	return true;
}
