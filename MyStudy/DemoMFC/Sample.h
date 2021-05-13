#pragma once
#include "myCore.h"
#include "myHeightMap.h"
#include "myQuadTree.h"
#include "myMouse.h"
#include "myFbxLoader.h"
#include "myMiniMap.h"
#include "myStagingTex.h"


class Sample : public myCore
{
public:
	shared_ptr<myModelObject>	m_pTargetObject;
	vector<shared_ptr<myModelObject>> m_DrawList;

	myCamera*		m_pTopCamera;

	myMiniMap*		m_pMiniMap;
	myGameObject*	m_pMiniMapObj;

	myMiniMap*		m_pHeightMini;
	myGameObject*	m_pHeightMiniObj;

	myStagingTex	m_HeightTex;
	myStagingTex	m_NormalTex;

	myMap*	m_pMap;
	myGameObject*	m_pMapObj;

	myQuadTree		m_QuadTree;
	myMouse			m_Mouse;

	vector<myNode*> m_SelectNodeList;
	vector<myNode*> m_ControlNodeList;
	bool			m_isCreate;
	bool			m_isPicking;
public:
	bool SetHeightTex(ID3D11DeviceContext*   pImmediateContext, ID3D11Texture2D* pTexDest);
	bool SetHeightTex(ID3D11Texture2D* pTexDest, Vector2& vLT, Vector2& vRB);
	bool SetNormalTex(ID3D11DeviceContext*   pImmediateContext, ID3D11Texture2D* pTexDest);
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
};