#pragma once
#include "myParsingScript.h"
class myScene
{
public:
	myObject*					m_pObject;
	std::vector<myObjectInfo*>	m_vecObjInfo;
	std::vector<myObjectInfo*>	m_vecEnemy;
	std::vector<myObjectInfo*>	m_vecUI;
	std::vector<myObjectInfo*>	m_vecPlayer;
	std::vector<myObjectInfo*>	m_vecEffect;
	int							m_iSceneID;
	bool						m_bAbleChange;
	int							m_iValue;
public:
	virtual bool				Load(myParsingScript* Parser, const TCHAR* szFileName);
	virtual void				DistributeInfo(int iPoolCount);
	virtual myPoint				GetPlayerPos();
public:
	virtual bool				Reset();
	virtual bool				Init();
	virtual bool				Frame();
	virtual bool				Render();
	virtual bool				Release();
public:
	myScene();
	virtual ~myScene();
};

