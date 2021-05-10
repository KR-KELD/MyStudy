#pragma once
#include "myCore.h"
#include "myHeightMap.h"
#include "myQuadTree.h"
#include "myMouse.h"
class Sample : public myCore
{
public:
	myQuadTree		m_QuadTree;
	myMouse			m_Mouse;
	myMap*			m_Map;
	vector<myNode*> m_SelectNodeList;
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
};