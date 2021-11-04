#include "myFPSController.h"
#include "myInput.h"
DECLARE_COMPONENT(myFPSController)

bool myFPSController::Controller()
{
	m_eMoveState = myEMoveState::MOVE_STOP;
	if (g_Input.GetKey(VK_SHIFT) == KEY_HOLD)
	{
		m_fPower = 2.0f;
		m_bRun = true;
	}
	if (g_Input.GetKey(VK_SHIFT) == KEY_UP)
	{
		m_bRun = false;
	}
	if (g_Input.GetKey('W') == KEY_HOLD)
	{
		m_eMoveState = myEMoveState::MOVE_FRONT;
		m_pTransform->FrontMovement(m_fPower);
	}
	if (g_Input.GetKey('S') == KEY_HOLD)
	{
		m_eMoveState = myEMoveState::MOVE_BACK;
		m_pTransform->FrontMovement(-m_fPower);
	}
	if (g_Input.GetKey('A') == KEY_HOLD)
	{
		m_eMoveState = myEMoveState::MOVE_LEFT;
		m_pTransform->RightMovement(-m_fPower);
	}
	if (g_Input.GetKey('D') == KEY_HOLD)
	{
		m_eMoveState = myEMoveState::MOVE_RIGHT;
		m_pTransform->RightMovement(m_fPower);
	}
	if (g_Input.GetKey('Q') == KEY_HOLD)
	{
		m_eMoveState = myEMoveState::MOVE_UP;
		m_pTransform->UpMovement(m_fPower);
	}
	if (g_Input.GetKey('E') == KEY_HOLD)
	{
		m_eMoveState = myEMoveState::MOVE_DOWN;
		m_pTransform->UpMovement(-m_fPower);
	}
	m_fPower = 1.0f;
	return true;
}
