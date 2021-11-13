#include "myObjManager.h"

bool myObjManager::Init()
{
	m_iObjectID = 0;
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
	m_SceneObj.PreRender(g_pImmediateContext);
	return true;
}

bool myObjManager::Render()
{
	m_SceneObj.Render(g_pImmediateContext);
	return true;
}

bool myObjManager::PostRender()
{
	m_SceneObj.PostRender(g_pImmediateContext);
	return true;
}

bool myObjManager::Release()
{
	m_SceneObj.Release();
	m_SubObj.Release();
	//for (m_ObjIter = m_ObjectList.begin(); m_ObjIter != m_ObjectList.end(); m_ObjIter++)
	//{
	//	m_ObjIter->second->Release();
	//}
	m_ObjectList.clear();
	return true;
}

myObjManager::myObjManager()
{

}

myObjManager::~myObjManager()
{

}
