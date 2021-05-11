#pragma once
#include "myCore.h"
#include "myHeightMap.h"
#include "myQuadTree.h"
#include "myMouse.h"
#include "myFbxLoader.h"



class Sample : public myCore
{
public:
	shared_ptr<myModelObject>	m_pTargetObject;
	vector<shared_ptr<myModelObject>> m_DrawList;
	myQuadTree		m_QuadTree;
	myMouse			m_Mouse;
	myMap*			m_Map;
	vector<myNode*> m_SelectNodeList;
	bool			m_isCreate;
public:
	bool SetTargetObject(string strName);
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
};