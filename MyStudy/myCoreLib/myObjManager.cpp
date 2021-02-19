#include "myObjManager.h"

bool myObjManager::Init()
{
	m_ObjectContainer.Init();
	return true;
}

bool myObjManager::Frame()
{
	m_ObjectContainer.Frame();
	return true;
}

bool myObjManager::Render()
{
	m_ObjectContainer.Render();
	return true;
}

bool myObjManager::Release()
{
	m_ObjectContainer.Release();
	return true;
}

myObjManager::myObjManager()
{

}

myObjManager::~myObjManager()
{

}
