#include "myEffect.h"

bool myEffect::Init()
{
	myObject::Init();
	return true;
}

bool myEffect::Frame()
{
	myObject::Frame();
	SetPos(m_pObjInfo->ptPos);
	return true;
}

bool myEffect::Render()
{
	myObject::Render();
	return true;
}

bool myEffect::Release()
{
	myObject::Release();
	return true;
}

myEffect::myEffect()
{
}

myEffect::~myEffect()
{
}
