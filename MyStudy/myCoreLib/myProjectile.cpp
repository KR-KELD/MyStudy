#include "myProjectile.h"

bool myProjectile::Move()
{
	if (m_pObjInfo == nullptr)
		return false;
	m_pObjInfo->ptPos = m_pObjInfo->ptPos + m_pObjInfo->ptDir *
		m_pObjInfo->fSpeed * g_fSecondPerFrame;

	myPoint dir = m_pObjInfo->ptDest - m_pObjInfo->ptPos;
	float dist = dir.Length();

	if (dist < 10.0f)
	{
		m_pObjInfo->bDead = true;
	}
	return true;
}

bool myProjectile::Init()
{
	myObject::Init();
	return true;
}

bool myProjectile::Frame()
{
	myObject::Frame();
	Move();
	// 추적 알고리즘 추가
	SetPos(m_pObjInfo->ptPos);
	return true;
}

bool myProjectile::Render()
{
	myObject::Render();
	return true;
}

bool myProjectile::Release()
{
	myObject::Release();
	return true;
}

myProjectile::myProjectile()
{
}

myProjectile::~myProjectile()
{
}