#pragma once
#include "myCore.h"
#include "myFigure.h"

struct myDataCB
{
	myMatrix  matWorld;
	myMatrix  matView;
	myMatrix  matProject;
	float vColor[4];
	float vTime[4];
};

class Sample : public myCore
{
public:
	myBox			m_Box;
public:
	float			m_CameraAngleX = 0.0f;
	float			m_CameraAngleY = 0.0f;
	myMatrix		m_matWorld;
	myMatrix		m_matView;
	myMatrix		m_matProj;
	myVector3		m_vCameraPos = { 0,0,-5 };
	myVector3		m_vCameraTarget = { 0,0,0 };
public:
	myDataCB					m_cbData;
	std::vector<MY_VERTEX>		m_VertexList;
	std::vector<DWORD>			m_IndexList;
	ID3D11Buffer*				m_pVertexBuffer;
	ID3D11Buffer*				m_pIndexBuffer;
	ID3D11Buffer*				m_pConstantBuffer;
	ID3D11InputLayout*			m_pInputLayout;
	ID3D11VertexShader*			m_pVertexShader;
	ID3D11PixelShader*			m_pPixelShader;
	ID3D11RasterizerState*		m_pRSSolidBack;
	ID3D11RasterizerState*		m_pRSWireBack;
	ID3D11RasterizerState*		m_pRS;
	ID3D11ShaderResourceView*	m_pTextureSRV;
	ID3D11SamplerState*			m_pWrapLinear;
	D3D11_FILL_MODE				m_FillMode;
	D3D11_CULL_MODE				m_CullMode;
	//ID3D11RasterizerState*	m_pRS;
public:
	ID3D11DepthStencilView*		m_pDSV;
	ID3D11DepthStencilState*	m_pDSS;
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
	bool PreRender() override;
public:
	bool MakeBox();
public:
	void CompilerCheck(ID3DBlob* pErrorMsgs);
	void SetRasterizerState();
};

