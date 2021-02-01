#pragma once
#include "myStd.h"
#include "myEffect.h"
#include "myProjectile.h"
#include "myParsingScript.h"

class myObjectPoolManager : public SingleTon<myObjectPoolManager>
{
	friend SingleTon<myObjectPoolManager>;
	typedef vector<myObjectInfo*>					ObjectPool;
	typedef std::map<wstring, ObjectPool>::iterator ObjPoolIter;
public:
	std::map<wstring, ObjectPool>	m_mapEffectPool;
	std::map<wstring, ObjectPool>	m_mapProjectilePool;
	ObjPoolIter						m_iterPool;

public:
	bool			AddEffectPool(wstring strName, vector<myObjectInfo*> vecObjPool);
	bool			AddProjectilePool(wstring strName, vector<myObjectInfo*> vecObjPool);
	myObjectInfo*	GetEffect(wstring strName);
	myObjectInfo*	GetProjectile(wstring strName);
	bool			StartEffect(wstring strName, myPoint ptPos, vector<myObjectInfo*>* playPool);
	bool			StartProjectile(wstring strName, myPoint ptPos, myPoint ptDest, vector<myObjectInfo*>* playPool);
public:
	bool			Init();
	bool			Frame();
	bool			Render();
	bool			Release();
private:
	myObjectPoolManager();
public:
	~myObjectPoolManager();
};
#define g_ObjPoolMgr myObjectPoolManager::GetInstance()

