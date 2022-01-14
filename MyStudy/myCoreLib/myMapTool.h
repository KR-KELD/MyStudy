#pragma once
#include "myGameObject.h"
#include "myBaseObject.h"
#include "myHeightMap.h"
#include "myQuadTree.h"
#include "myMouse.h"
#include "myStagingTex.h"
#include "myFbxLoader.h"
#include "myCamera.h"
#include "myComputeShader.h"

enum ToolType
{
	TOOL_TOPOLOGY = 0,
	TOOL_SPLAT,
	TOOL_OBJECT,

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

enum ObjectEditType
{
	OBJECT_TRANS = 300,
	OBJECT_SCALE,
	OBJECT_ROTATE,
	OBJECT_CREATE,
	OBJECT_DELETE,
};

struct SplatBuf
{
	Vector2 vPickPos;
	Vector2 vRadius; //x Out y In
	int    iIndex;// rgba
	float  fTexWidth;
	float  fTexHeight;
};


class myMapTool : public myComponent
{
public:
	int m_iTexSizeX = 1024;
	int m_iTexSizeY = 1024;
public:
	myHeightMap*	m_pMap;
	myQuadTree*		m_pQuadTree;
	myMouse			m_Mouse;
	myStagingTex	m_HeightTex;
	myStagingTex	m_NormalTex;

	myComputeShader m_SplatCS;
	SplatBuf		m_sbSplat[1];
public:
	ID3D11ShaderResourceView* m_pSplatTex[4];
public:
	vector<myNode*> m_SelectNodeList;
	myNode*			m_pSelectNode = nullptr;
	vector<myNode*> m_ControlNodeList;
public:
	ToolType		m_eMakingMode;
	TopologyType	m_eTopologyType;
	SplatType		m_eSplatType;
	ObjectEditType  m_eObjEditType;
public:
	shared_ptr<myModelObject>	m_pTargetObject;
	SampleIns*		m_pTargetIns;
	vector<myModelObject*> m_BaseList;
	//vector<myModelObject*> m_DrawList;
public:
	MY_SPHERE	m_PickSphere;
	Vector2		m_vBrushRad; // x Out y In
	float		m_fSpeed;
	bool		m_isPicking;
	bool		m_isUpdateData;
	bool		m_isUpdatePick;
	bool		m_isSelectObject;
	Vector3		m_vTemp;
	vector<SampleIns*> InstanceList;
public:
	WCHAR	m_szToolType[256];
	WCHAR	m_szToolMode[256];
public:
	bool	Init();
	bool	Frame() override;
	bool	Render(ID3D11DeviceContext*	pd3dContext) override;
	bool	Release() override;
public:
	void	CreateTex(int iTexSize = 1024);
	void	EditTopology(Vector3& vPick);
	void	EditSplatting(Vector3& vPick);
	void	SetMode(int iMode);
	void	EditTerrain(Vector3& vPick);
	void	EditObject(Vector3& vPick);
	bool	SetHeightTex(ID3D11DeviceContext * pImmediateContext, ID3D11Texture2D * pTexDest);
	//bool	SetNormalTex(ID3D11DeviceContext*   pImmediateContext, ID3D11Texture2D* pTexDest, Vector3& vPick , Vector2& vLT, Vector2& vRB);
	bool	ResetTex(ID3D11DeviceContext*   pImmediateContext, ID3D11Texture2D* pTexDest);
	void	DebugText();
public:
	myMapTool(myHeightMap* pMap, myQuadTree* pQuadTree);
	virtual ~myMapTool();
};

