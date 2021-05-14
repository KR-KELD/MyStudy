#include "myObjManager.h"

bool myObjManager::Init()
{
	m_SceneObj.Init();
	m_SubObj.Init();
	return true;
}

bool myObjManager::PreFrame()
{
	m_SceneObj.PreFrame();
	return true;
}

bool myObjManager::Frame()
{
	m_SceneObj.Frame();
	return true;
}

bool myObjManager::PostFrame()
{
	m_SceneObj.PostFrame();
	return true;
}

bool myObjManager::PreRender()
{
	m_SceneObj.PreRender();
	return true;
}

bool myObjManager::Render()
{
	m_SceneObj.Render();
	return true;
}

bool myObjManager::PostRender()
{
	m_SceneObj.PostRender();
	return true;
}

bool myObjManager::Release()
{
	m_SceneObj.Release();
	m_SubObj.Release();
	return true;
}

myObjManager::myObjManager()
{

}

myObjManager::~myObjManager()
{

}
