#include "myButton.h"

//bool myButton::MoveRect(myPoint & p)
//{
//	//m_pObjInfo->rtDest.left = (LONG)p.x;
//	//m_pObjInfo->rtDest.top = (LONG)p.y;
//
//	//m_pObjInfo->rtCollider.left = (LONG)p.x;
//	//m_pObjInfo->rtCollider.top = (LONG)p.y;
//	//m_pObjInfo->rtCollider.right = (LONG)(p.x + m_pObjInfo->iCollWidth);
//	//m_pObjInfo->rtCollider.bottom = (LONG)(p.y + m_pObjInfo->iCollHeight);
//	return true;
//}

bool myButton::Action()
{
	return true;
}

bool myButton::Reset()
{
	return true;
}

bool myButton::Init()
{
	myUI::Init();
	m_iState = IDLE;
	return true;
}

bool myButton::Frame()
{
	//m_iState = IDLE;
	//if (myCollision::PtInRectWH(m_pObjInfo->rtDest, g_Input.GetMouse()))
	//{
	//	m_iState = OVER;
	//	if (g_Input.GetKey(VK_LBUTTON) == KEY_HOLD)
	//	{
	//		m_iState = CLICK;
	//	}
	//	if (g_Input.GetKey(VK_LBUTTON) == KEY_UP)
	//	{
	//		Action();
	//	}
	//}
	//m_pObjInfo->iRectIndex = (int)m_iState;
	//SetPos(m_pObjInfo->ptPos);
	//SetSpriteRt(m_pObjInfo->strSpriteName);
	return true;
}

bool myButton::Render()
{
	myObject::Render();
	return true;
}

bool myButton::Release()
{
	myObject::Release();
	return true;
}
