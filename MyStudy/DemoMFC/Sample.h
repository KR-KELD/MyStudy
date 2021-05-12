#pragma once
#include "myCore.h"
#include "myHeightMap.h"
#include "myQuadTree.h"
#include "myMouse.h"
#include "myFbxLoader.h"
#include "myMiniMap.h"



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

	ComPtr<ID3D11Texture2D>			m_pStaging;
	ComPtr<ID3D11Texture2D>			m_pHeight;
	ComPtr<ID3D11ShaderResourceView> m_pSRVHeight;

	myMap*	m_pMap;
	myGameObject*	m_pMapObj;

	myQuadTree		m_QuadTree;
	myMouse			m_Mouse;

	vector<myNode*> m_SelectNodeList;
	vector<myNode*> m_ControlNodeList;
	bool			m_isCreate;
public:
	bool SetHeightTex(ID3D11DeviceContext*   pImmediateContext, ID3D11Texture2D* pTexDest);

	bool SetTargetObject(string strName);
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
};