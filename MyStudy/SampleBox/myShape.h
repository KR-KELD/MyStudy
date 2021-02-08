#pragma once
#include "myMatrix.h"
#include "WICTextureLoader.h"
struct P_VERTEX
{
	myVector3 p;
	myVector3 n;
	myVector4 c;
	myVector2 t;
	P_VERTEX() {}
	P_VERTEX(myVector3 p,
		myVector3 n,
		myVector4 c,
		myVector2 t)
	{
		this->p = p;
		this->n = n;
		this->c = c;
		this->t = t;
	}
};
struct PNCT_VERTEX
{
	myVector3		p;
	myVector3		n;
	myVector4		c;
	myVector2     t;
	bool operator == (const PNCT_VERTEX & Vertex)
	{
		if (p == Vertex.p  && n == Vertex.n && 	c == Vertex.c  &&	t == Vertex.t)
		{
			return true;
		}
		return  false;
	}
	PNCT_VERTEX() {}
	PNCT_VERTEX(myVector3		vp,
		myVector3		vn,
		myVector4		vc,
		myVector2     vt)
	{
		p = vp, n = vn, c = vc, t = vt;
	}
};

struct myDataCB
{
	myMatrix  matWorld;
	myMatrix  matView;
	myMatrix  matProject;
	float vColor[4];
	float vTime[4];
};

class myShape
{
public:
	ID3D11Device* m_pd3dDevice;
	ID3DBlob*	  m_pVSObj;
	UINT		  m_iTopology;
	C_STR		  m_szVertexShader;
	C_STR		  m_szPixelShader;
public:
	myMatrix    m_matWorld;
	myMatrix    m_matView;
	myMatrix    m_matProj;
public:
	myDataCB					m_cbData;
	std::vector<PNCT_VERTEX>	m_VertexList;
	std::vector<DWORD>		m_IndexList;
	ID3D11Buffer*			m_pVertexBuffer;
	ID3D11Buffer*			m_pIndexBuffer;
	ID3D11Buffer*			m_pConstantBuffer;
	ID3D11InputLayout*		m_pInputLayout;
	ID3D11VertexShader*		m_pVertexShader;
	ID3D11PixelShader*		m_pPixelShader;
	ID3D11ShaderResourceView* m_pTextureSRV;
public:
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	SetMatrix(myMatrix* pWorld,
		myMatrix* pView,
		myMatrix* pProj);
	virtual bool    Update(ID3D11DeviceContext*	pd3dContext);
	virtual bool	Render(ID3D11DeviceContext*	pd3dContext);
	virtual bool	Relase();
	virtual bool    CreateVertexData();
	virtual bool    CreateIndexData();
	virtual bool    CreateVertexBuffer();
	virtual bool    CreateIndexBuffer();
	virtual bool    CreateConstantBuffer();
	virtual bool	LoadShader(T_STR szVS, T_STR szPS);
	virtual bool	CreateInputLayout();
	virtual bool	LoadTexture(T_STR szTex);
	virtual bool	Create(ID3D11Device* pDevice,
		T_STR szVS, T_STR szPS,
		T_STR	szTex);
	void	CompilerCheck(ID3DBlob* pErrorMsgs);
public:
	myShape();
	virtual ~myShape();
};

class myShapeBox : public myShape
{
public:
	virtual bool    CreateVertexData()override;
	virtual bool    CreateIndexData()override;
public:
	myShapeBox();
	virtual ~myShapeBox();
};

class myShapePlane : public myShape
{
public:
	virtual bool    CreateVertexData()override;
	virtual bool    CreateIndexData()override;
public:
	myShapePlane();
	virtual ~myShapePlane();
};

class myShapeLine : public myShape
{
public:
	bool Draw(ID3D11DeviceContext* pd3dContext,
		myVector3 p, myVector3 e, myVector4 c = myVector4(1, 0, 0, 1));
public:
	virtual bool    CreateVertexData()override;
	virtual bool    CreateIndexData()override;
public:
	myShapeLine();
	virtual ~myShapeLine();
};
