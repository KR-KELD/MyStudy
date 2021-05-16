#pragma once
#include "myGameObject.h"
#include "myBaseObject.h"
#include "myHeightMap.h"
#include "myQuadTree.h"
#include "myMouse.h"
#include "myStagingTex.h"
enum ToolType
{
	TOOL_TOPOLOGY = 0,
	TOOL_SPLAT,
};

enum TopologyType
{
	TERRAIN_UP = 100,
	TERRAIN_DOWN,
	TERRAIN_FLAT,
};
enum SplatType
{
	SPLAT_TEX_01 = 200,
	SPLAT_TEX_02,
	SPLAT_TEX_03,
	SPLAT_TEX_04,
	SPLAT_TEX_NONE = 299,
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
	ID3D11ShaderResourceView* m_pSplatTex[4];
public:
	vector<myNode*> m_SelectNodeList;
	vector<myNode*> m_ControlNodeList;
public:
	ToolType		m_eMakingMode;
	TopologyType	m_eTopologyType;
	SplatType		m_eSplatType;
public:

	MY_SPHERE	m_PickSphere;
	float		m_fOutRad;
	float		m_fInnerRad;
	float		m_fSpeed;
	bool		m_isPicking;
	bool		m_isChangeData;
public:
	bool	SetTexture(int iTexSize = 1024);
	bool	Init();
	bool	Frame() override;
	bool	Render() override;
	bool	Release() override;
	void	SetMode(int iMode);
	void	EditTerrain();
	bool	SetHeightTex(ID3D11DeviceContext * pImmediateContext, ID3D11Texture2D * pTexDest);
	bool	SetNormalTex(ID3D11DeviceContext*   pImmediateContext, ID3D11Texture2D* pTexDest, Vector3& vPick);
	bool	ResetTex(ID3D11DeviceContext*   pImmediateContext, ID3D11Texture2D* pTexDest);
public:
	myMapTool(myMap* pMap, myQuadTree* pQuadTree);
	virtual ~myMapTool();
};

