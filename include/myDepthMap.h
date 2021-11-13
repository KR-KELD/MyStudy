#pragma once
#include "myGameObject.h"
#include "myDxRT.h"

struct cbDepthMap
{
	//일단은 그냥 하고 여러개의 라이트를 쓰면 배열로 만들기
	//여러개의 라이트를 
	Matrix g_matShadow;
};

class myDepthMap : myComponent
{
public:
	DEFINE_COMPONENT(myDepthMap, myComponent, true)
public:
	ComPtr<ID3D11PixelShader>	m_pPSDepthMap;
	ComPtr<ID3D11VertexShader>	m_pVSDepthMap;
	ComPtr<ID3D11Buffer>		m_pCBDepthMap;
	ComPtr<ID3D11InputLayout>	m_pInputLayout;
	ID3DBlob*					m_pVSObj;
	cbDepthMap					m_cbData;
	myDxRT*						m_pRT;
	Matrix						m_matShadowTex;
public:
	bool Init() override;
	bool Frame() override;
	bool PreFrame() override;
	bool Render(ID3D11DeviceContext* pd3dContext) override;
	bool Release() override;
public:
	bool CreateDepthMapRT(int iTexWidth, int iTexHeight);
	bool CreateDepthVS(const TCHAR* szFileName);
	bool CreateDepthPS(const TCHAR* szFileName);
	HRESULT CreateInputLayout();
	HRESULT CreateShadowCB();
};

