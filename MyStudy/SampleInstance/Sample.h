#pragma once
#include "myCore.h"
#include "myModelViewCamera.h"
#include "myHeightMap.h"
#include "myMiniMap.h"
#include "myQuadTree.h"
#include "myFbxLoader.h"
#include "myMouse.h"

#include "myInstanceGraphics.h"

struct myModelInstance
{
	Vector3 vPos;
	float fTick;
};

class Sample : public myCore
{
public:
	myInstanceGraphics* m_pIns;
	myGameObject*		m_pInsObj;
	int					m_iNum;


	myFbxObj* m_pFbxObj;
	shared_ptr<myModelObject> m_pModelObj;

	myQuadTree		m_QuadTree;
	myMouse			m_Mouse;
	myMap*			m_Map;
	vector<myNode*> m_SelectNodeList;
	vector<myNode*> m_ControlNodeList;

	vector<myModelInstance> m_ModelList;
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
};

//walk 3-35
//idle1 36-101
//idle2 102-161