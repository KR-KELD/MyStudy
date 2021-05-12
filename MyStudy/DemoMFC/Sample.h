#pragma once
#include "myCore.h"
#include "myHeightMap.h"
#include "myQuadTree.h"
#include "myMouse.h"
#include "myFbxLoader.h"
#include "myMiniMap.h"



class Sample : public myCore
{
public:
	shared_ptr<myModelObject>	m_pTargetObject;
	vector<shared_ptr<myModelObject>> m_DrawList;

	myCamera*		m_pTopCamera;

	myMiniMap*		m_pMiniMap;
	myGameObject*	m_pMiniMapObj;

	myMap*			m_pMap;
	myGameObject*	m_pMapObj;

	myQuadTree		m_QuadTree;
	myMouse			m_Mouse;

	vector<myNode*> m_SelectNodeList;
	bool			m_isCreate;
public:
	bool SetTargetObject(string strName);
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
};