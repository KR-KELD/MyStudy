#pragma once
#include "myCore.h"
#include "myFbxLoader.h"
#include "myMiniMap.h"
#include "myMapTool.h"

enum class eTypeMinimap
{
	RENDER_NORMAL = 0,
	RENDER_DEPTH,
	RENDER_HEIGHT,
	RENDER_NONE = 99,
};

class Sample : public myCore
{
public:
	myCamera*		m_pTopCamera;
	myMiniMap*		m_pMiniMap;
	myHeightMap*	m_pMap;
	myMapTool*		m_pMapTool;
	myQuadTree		m_QuadTree;

	eTypeMinimap	m_eMinimapType;

	bool			m_isDebugText = true;
	bool			m_isCreate;
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
};