#pragma once
#include "myGameObject.h"
#include "myDxRT.h"

struct cbDepthMap
{
	//�ϴ��� �׳� �ϰ� �������� ����Ʈ�� ���� �迭�� �����
	//�������� ����Ʈ�� 
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

