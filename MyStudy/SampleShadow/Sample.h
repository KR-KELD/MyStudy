#pragma once
#include "myCore.h"
#include "myMap.h"
#include "myDxRT.h"
#include "myMiniMap.h"

struct cbShadowMatrix
{
	Matrix g_matShadow;
};

class Sample : public myCore
{
public:
	myFbxObj* m_pFbxObj;
	shared_ptr<myModelObject> m_pModelObj;
	myMap*	m_pMap;
	myCamera* m_pLight;

	ID3D11PixelShader*  m_pPSShadow;
	ID3D11PixelShader*  m_pPSShadowMap;

	myMiniMap* m_pShadowMinimap;
	myDxRT* m_pShadowMapRT;
	Matrix m_matShadowTex;
	cbShadowMatrix   m_cbShadow;
	ID3D11Buffer*    m_pShadowCB = nullptr;
public:
	HRESULT  CreateShadowCB();

	bool Init() override;
	bool Frame() override;
	bool PreFrame() override;
	bool Render() override;
	bool Release() override;
};