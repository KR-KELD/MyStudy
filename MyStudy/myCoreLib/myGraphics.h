#pragma once
#include "myDxState.h"
#include "myGameObject.h"
#include "myTextureManager.h"

struct P_VERTEX
{
	Vector3 p;
	Vector3 n;
	Vector4 c;
	Vector2 t;
	P_VERTEX() {}
	P_VERTEX(
		Vector3 p,
		Vector3 n,
		Vector4 c,
		Vector2 t)
	{
		this->p = p;
		this->n = n;
		this->c = c;
		this->t = t;
	}
};
struct PNCT_VERTEX
{
	Vector3 p;
	Vector3 n;
	Vector4 c;
	Vector2 t;
	bool operator == (const PNCT_VERTEX & Vertex)
	{
		if (p == Vertex.p  && n == Vertex.n && 	c == Vertex.c  &&	t == Vertex.t)
		{
			return true;
		}
		return  false;
	}
	PNCT_VERTEX() {}
	PNCT_VERTEX(
		Vector3 vp,
		Vector3 vn,
		Vector4 vc,
		Vector2 vt)
	{
		p = vp, n = vn, c = vc, t = vt;
	}
};

struct myDataCB
{
	Matrix  matWorld;
	Matrix  matView;
	Matrix  matProject;
	float vColor[4];
	float vTime[4];
};

class myGraphics : public myComponent
{
public:
	DEFINE_COMPONENT(myGraphics, myComponent, true);
public:
	ID3D11Device*				m_pd3dDevice;
	ID3DBlob*					m_pVSObj;
	UINT						m_iTopology;
	C_STR						m_szVertexShader;
	C_STR						m_szPixelShader;
public:
	Matrix						m_matWorld;
	Matrix						m_matView;
	Matrix						m_matProj;
public:
	myDataCB					m_cbData;
	std::vector<PNCT_VERTEX>	m_VertexList;
	std::vector<DWORD>			m_IndexList;
	ID3D11Buffer*				m_pVertexBuffer;
	ID3D11Buffer*				m_pIndexBuffer;
	ID3D11Buffer*				m_pConstantBuffer;
	ID3D11InputLayout*			m_pInputLayout;
	ID3D11VertexShader*			m_pVertexShader;
	ID3D11PixelShader*			m_pPixelShader;
	myTexture*					m_pTexture;
public:
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	SetMatrix(Matrix* pWorld, Matrix* pView, Matrix* pProj);
	virtual bool    Update(ID3D11DeviceContext*	pd3dContext);
	virtual bool	PreRender(ID3D11DeviceContext*	pd3dContext);
	virtual bool	Render(ID3D11DeviceContext*	pd3dContext);
	virtual bool	PostRender(ID3D11DeviceContext*	pd3dContext);
	virtual bool	Release();
	virtual bool    CreateVertexData(Vector3 vCenter, float fRange);
	virtual bool    CreateVertexData();
	virtual bool    CreateIndexData();
	virtual bool    CreateVertexBuffer();
	virtual bool    CreateIndexBuffer();
	virtual bool    CreateConstantBuffer();
	virtual bool	LoadShader(T_STR szVS, T_STR szPS);
	virtual bool	CreateInputLayout();
	virtual bool	LoadTexture(T_STR szTex);
	virtual bool	Create(ID3D11Device* pDevice, T_STR szVS, T_STR szPS, T_STR	szTex);
	void	CompilerCheck(ID3DBlob* pErrorMsgs);
public:
	myGraphics();
	~myGraphics();
};

