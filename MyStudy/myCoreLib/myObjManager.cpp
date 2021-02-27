#include "myObjManager.h"

bool myObjManager::Init()
{
	m_ObjectContainer.Init();
	return true;
}

bool myObjManager::PreFrame()
{
	m_ObjectContainer.PreFrame();
	return true;
}

bool myObjManager::Frame()
{
	m_ObjectContainer.Frame();
	return true;
}

bool myObjManager::PostFrame()
{
	m_ObjectContainer.PostFrame();
	return true;
}

bool myObjManager::PreRender()
{
	m_ObjectContainer.PreRender();
	return true;
}

bool myObjManager::Render()
{
	m_ObjectContainer.Render();
	return true;
}

bool myObjManager::PostRender()
{
	m_ObjectContainer.PostRender();
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
