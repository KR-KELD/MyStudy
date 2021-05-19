#pragma once
#include "myCore.h"
#include "myFbxLoader.h"
#include "myMiniMap.h"
#include "myMapTool.h"


class Sample : public myCore
{
public:
	myCamera*		m_pTopCamera;
	myMiniMap*		m_pMiniMap;
	myMiniMap*		m_pHeightMini;
	myHeightMap*	m_pMap;
	myMapTool*		m_pMapTool;
	myQuadTree		m_QuadTree;

	bool			m_isCreate;
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
};