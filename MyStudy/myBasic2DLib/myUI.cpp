#include "myUI.h"

bool myUI::MoveRect(myPoint & p)
{
	m_pObjInfo->rtDest.left = (LONG)(p.x - g_CameraMgr.GetOffset().x);
	m_pObjInfo->rtDest.top = (LONG)(p.y - g_CameraMgr.GetOffset().y);

	m_pObjInfo->rtCollider.left = (LONG)p.x;
	m_pObjInfo->rtCollider.top = (LONG)p.y;
	m_pObjInfo->rtCollider.right = (LONG)(p.x + m_pObjInfo->iCollWidth);
	m_pObjInfo->rtCollider.bottom = (LONG)(p.y + m_pObjInfo->iCollHeight);
	return true;
}

bool myUI::Action()
{
	return true;
}

bool myUI::Reset()
{
	return true;
}

bool myUI::Init()
{
	myObject::Init();
	return true;
}

bool myUI::Frame()
{
	myObject::Frame();
	return true;
}

bool myUI::Render()
{
	myObject::Render();
	return true;
}

bool myUI::Release()
{
	myObject::Release();
	return true;
}
