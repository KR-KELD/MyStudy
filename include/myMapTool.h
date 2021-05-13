#pragma once
#include "myGameObject.h"
#include "myBaseObject.h"
#include "myHeightMap.h"
#include "myQuadTree.h"
#include "myMouse.h"
enum ToolType
{
	TERRAIN_UP = 0,
	TERRAIN_DOWN,
	TERRAIN_FLAT,
};

class myMapTool : public myComponent
{
public:
	myMap*		m_pMap;
	myQuadTree* m_pQuadTree;
	myMouse		m_Mouse;
public:
	vector<myNode*> m_SelectNodeList;
	vector<myNode*> m_ControlNodeList;
public:
	ToolType	m_eMakingMode;
	MY_SPHERE	m_PickSphere;
	float		m_fRadius;
	float		m_fSpeed;
	bool		m_isPicking;
public:
	bool	Init();
	bool Frame() override;
	bool Render() override;
	bool Release() override;
	void	SetMode(int iMode);
	void	EditTerrain();
public:
	myMapTool(myMap* pMap, myQuadTree* pQuadTree);
	virtual ~myMapTool();
};

