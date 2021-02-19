#pragma once
#include "myGameObject.h"

class myObjManager : public SingleTon<myObjManager>
{
	friend class SingleTon<myObjManager>;
public:
	myGameObject	m_ObjectContainer;
public:
	////존재이유 고민해보기
	template <class Component_T>
	myGameObject*	CreateComponentInObj(wstring strObjName, Component_T* component)
	{
		myGameObject* obj = m_ObjectContainer.Add(strObjName);
		obj->InsertComponent(component);
		//component->Set(obj);
		return obj;
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
