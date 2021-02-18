#include "myGameObject.h"

bool myGameObject::Init()
{
	return true;
}

bool myGameObject::PreFrame()
{
	return true;
}

bool myGameObject::Frame()
{
	return true;
}

bool myGameObject::PostFrame()
{
	return true;
}

bool myGameObject::PreRender()
{
	return true;
}

bool myGameObject::Render()
{
	return true;
}

bool myGameObject::PostRender()
{
	return true;
}

void myGameObject::Update()
{
}

void myGameObject::Reset()
{
}

bool myGameObject::Action()
{
	return true;
}

bool myGameObject::Release()
{

	for (m_iter = components.begin();
		m_iter != components.end();
		m_iter++)
	{
		(*m_iter).second->Release();
		delete (*m_iter).second;
	}
	components.clear();
	return true;
}

myGameObject::myGameObject()
{
}

myGameObject::~myGameObject()
{
}
