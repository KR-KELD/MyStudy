#pragma once
#include "myCore.h"
#include "myFbxLoader.h"
#include "myMiniMap.h"
#include "myMapTool.h"


class Sample : public myCore
{
public:
	shared_ptr<myModelObject>	m_pTargetObject;
	vector<shared_ptr<myModelObject>> m_DrawList;

	myCamera*		m_pTopCamera;
	myMiniMap*		m_pMiniMap;
	myMiniMap*		m_pHeightMini;
	myMap*			m_pMap;
	myMapTool*		m_pMapTool;
	myQuadTree		m_QuadTree;

	vector<myNode*> m_SelectNodeList;
	vector<myNode*> m_ControlNodeList;
	bool			m_isCreate;
	bool			m_isPicking;
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
};