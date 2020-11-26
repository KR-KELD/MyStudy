#pragma once
#include "myObject.h"
class myObjManager : public SingleTon<myObjManager>
{
	friend class SingleTon<myObjManager>;
private:
	std::map<wstring, myObject*>				m_List;
	std::map<wstring, myObject*>::iterator		m_iter;
public:
	bool			Add(wstring strName, myObject* pObj);
	myObject*		GetPtr(wstring strName);
	myObject*		GetPtrToLink(wstring strName, myObjectInfo* pInfo);
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
