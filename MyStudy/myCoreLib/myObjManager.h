#pragma once
#include "myGameObject.h"

class myObjManager : public SingleTon<myObjManager>
{
	friend class SingleTon<myObjManager>;
public:
	myGameObject	m_ObjectContainer;
public:
	template <class Component_T>
	myGameObject*	CreateObjComponent(wstring strObjName, Component_T* pComponent)
	{
		myGameObject* obj = m_ObjectContainer.Add(strObjName);
		obj->InsertComponent(pComponent);
		pComponent->Init();
		return obj;
	}
	template <class Component_T>
	myGameObject*	InsertComponentInObj(wstring strObjName, Component_T* pComponent)
	{
		myGameObject* obj = m_ObjectContainer.GetGameObject(strObjName);
		if (obj == nullptr)
		{
			return nullptr;
		}
		obj->InsertComponent(pComponent);
	}
public:
	bool			Init();
	bool			Frame();
	bool			Render();
	bool			Release();
private:
	myObjManager();
public:
	~myObjManager();
};
#define g_ObjMgr myObjManager::GetInstance()
#define g_GameObject myObjManager::GetInstance().m_ObjectContainer
