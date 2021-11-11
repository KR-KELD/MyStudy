#pragma once
#include "myGameObject.h"
#include "myDxRT.h"

struct cbShadowMatrix
{
	Matrix g_matShadow;
};

class myDepthMap : myComponent
{
public:
	DEFINE_COMPONENT(myDepthMap, myComponent, true)
public:
	ComPtr<ID3D11PixelShader>	m_pPSShadowMap;
	ComPtr<ID3D11Buffer>		m_pShadowCB;
	cbShadowMatrix				m_cbShadow;
	myDxRT*						m_pShadowMapRT;
	Matrix						m_matShadowTex;
public:

};

