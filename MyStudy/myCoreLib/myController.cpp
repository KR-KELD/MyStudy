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
	if (g_Input.GetKey(VK_SHIFT) == KEY_HOLD)
	{
		m_fPower = 2.0f;
	}
	if (g_Input.GetKey('W') == KEY_HOLD)
	{
		m_pTransform->FrontMovement(m_fPower);
	}
	if (g_Input.GetKey('S') == KEY_HOLD)
	{
		m_pTransform->FrontMovement(-m_fPower);
	}
	if (g_Input.GetKey('A') == KEY_HOLD)
	{
		m_pTransform->RightMovement(-m_fPower);
	}
	if (g_Input.GetKey('D') == KEY_HOLD)
	{
		m_pTransform->RightMovement(m_fPower);
	}
	if (g_Input.GetKey('Q') == KEY_HOLD)
	{
		m_pTransform->UpMovement(m_fPower);
	}
	if (g_Input.GetKey('E') == KEY_HOLD)
	{
		m_pTransform->UpMovement(-m_fPower);
	}
	m_fPower = 1.0f;
	return true;
}
