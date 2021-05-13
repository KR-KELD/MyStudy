#pragma once
#include "myGameObject.h"
#include "myBaseObject.h"
#include "myHeightMap.h"
#include "myQuadTree.h"
#include "myMouse.h"
#include "myStagingTex.h"
enum ToolType
{
	TERRAIN_UP = 0,
	TERRAIN_DOWN,
	TERRAIN_FLAT,
};

class myMapTool : public myComponent
{
public:
	myMap*			m_pMap;
	myQuadTree*		m_pQuadTree;
	myMouse			m_Mouse;
	myStagingTex	m_HeightTex;
	myStagingTex	m_NormalTex;
public:
	vector<myNode*> m_SelectNodeList;
	vector<myNode*> m_ControlNodeList;
public:
	ToolType	m_eMakingMode;
	MY_SPHERE	m_PickSphere;
	float		m_fRadius;
	float		m_fSpeed;
	bool		m_isPicking;
	bool		m_isChangeData;
public:
	bool	Init();
	bool	Frame() override;
	bool	Render() override;
	bool	Release() override;
	void	SetMode(int iMode);
	void	EditTerrain();
	bool	SetHeightTex(ID3D11DeviceContext * pImmediateContext, ID3D11Texture2D * pTexDest);
	bool	SetHeightTex(ID3D11DeviceContext*   pImmediateContext, ID3D11Texture2D* pTexDest, Vector3& vLT, Vector3& vRB);
	bool	SetNormalTex(ID3D11DeviceContext*   pImmediateContext, ID3D11Texture2D* pTexDest);
public:
	myMapTool(myMap* pMap, myQuadTree* pQuadTree);
	virtual ~myMapTool();
};

