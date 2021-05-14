#pragma once
#include "myGameObject.h"

enum myObjectType
{
	OBJECT_SCENE = 0,
	OBJECT_SUB,

};

class myObjManager : public SingleTon<myObjManager>
{
	friend class SingleTon<myObjManager>;
public:
	myGameObject	m_SceneObj;
	myGameObject	m_SubObj;
public:
	template <class Component_T>
	myGameObject*	CreateObjComponent(wstring strObjName, Component_T* pComponent, myObjectType eObjType)
	{
		myGameObject* obj = nullptr;
		switch (eObjType)
		{
		case OBJECT_SCENE:
		{
			obj = m_SceneObj.Add(strObjName);
		}
			break;
		case OBJECT_SUB:
		{
			obj = m_SubObj.Add(strObjName);
		}
			break;
		default:
			break;
		}
		obj->InsertComponent(pComponent);
		pComponent->Init();
		return obj;
	}
	template <class Component_T>
	myGameObject*	InsertComponentInObj(wstring strObjName, Component_T* pComponent, myObjectType eObjType)
	{
		myGameObject* obj = nullptr;
		switch (eObjType)
		{
		case OBJECT_SCENE:
		{
			obj = m_SceneObj.GetGameObject(strObjName);
		}
		break;
		case OBJECT_SUB:
		{
			obj = m_SubObj.GetGameObject(strObjName);
		}
		break;
		default:
			break;
		}
		if (obj == nullptr)
		{
			return nullptr;
		}
		obj->InsertComponent(pComponent);
	}
public:
	bool	Init();
	bool	PreFrame();
	bool	Frame();
	bool	PostFrame();
	bool	PreRender();
	bool	Render();
	bool	PostRender();
	bool	Release();
private:
	myObjManager();
public:
	~myObjManager();
};
#define g_ObjMgr myObjManager::GetInstance()
#define g_RunGameObject myObjManager::GetInstance().m_SceneObj
#define g_SubGameObject myObjManager::GetInstance().m_SubObj
