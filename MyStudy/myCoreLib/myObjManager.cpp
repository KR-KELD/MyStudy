#include "myObjManager.h"

bool myObjManager::Add(wstring strName, myObject * pObj)
{
	if (pObj == nullptr || strName.empty())
		return false;
	m_iter = m_List.find(strName);
	if (m_iter != m_List.end())
	{
		(*m_iter).second->Release();
		m_List.erase(strName);
		delete (*m_iter).second;
	}
	m_List.insert(make_pair(strName, pObj));
	return true;
}

myObject * myObjManager::GetPtr(wstring strName)
{
	m_iter = m_List.find(strName);
	if (m_iter == m_List.end())
	{
		return nullptr;
	}
	return (*m_iter).second;
}

bool myObjManager::Init()
{
	return true;
}

bool myObjManager::Frame()
{
	return true;
}

bool myObjManager::Render()
{
	return true;
}

bool myObjManager::Release()
{
	for (m_iter = m_List.begin();
		m_iter != m_List.end();
		m_iter++)
	{
		(*m_iter).second->Release();
		SAFE_DELETE((*m_iter).second);
	}
	m_List.clear();
	return true;
}

myObjManager::myObjManager()
{

}

myObjManager::~myObjManager()
{
}
