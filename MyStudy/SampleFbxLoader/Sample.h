#pragma once
#include "myCore.h"
#include "myModelViewCamera.h"
#include "myHeightMap.h"
#include "myMiniMap.h"
#include "myQuadTree.h"
#include "myFbxLoader.h"
#include "myMouse.h"



class Sample : public myCore
{
public:
	myFbxObj* m_pFbxObj;
	shared_ptr<myModelObject> m_pModelObj;

	myQuadTree		m_QuadTree;
	myMouse			m_Mouse;
	myMap*			m_Map;
	vector<myNode*> m_SelectNodeList;
	vector<myNode*> m_ControlNodeList;
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
};

//walk 3-35
//idle1 36-101
//idle2 102-161