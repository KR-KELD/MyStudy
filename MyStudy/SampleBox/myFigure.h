#pragma once
#include "myMatrix.h"
#include <d3dcompiler.h>
#include "WICTextureLoader.h"
#pragma comment(lib, "d3dcompiler.lib")

struct MY_VERTEX
{
	myVector3 p;
	myVector3 n;
	myVector4 c;
	myVector2 t;
	MY_VERTEX() {}
	MY_VERTEX(myVector3 p, myVector3 n, myVector4 c, myVector2 t)
	{
		this->p = p;
		this->n = n;
		this->c = c;
		this->t = t;
	}
};

class myBaseFigure
{
public:
	//myMatrix			m_matWorld;
	//myMatrix			m_matView;
	//myMatrix			m_matProj;
	//ID3D11Buffer*		m_pVertexBuffer;
	//ID3D11Buffer*		m_pIndexBuffer;
	//ID3D11Buffer*		m_pConstantBuffer;
	vector<MY_VERTEX>	m_VertexList;
	vector<DWORD>		m_IndexList;
	myVector3			m_vCenter;
	float				m_fRange;
public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
};

class mySquare : public myBaseFigure
{
public:
	enum
	{
		LEFT_TOP = 0,
		RIGHT_TOP,
		LEFT_BOTTOM,
		RIGHT_BOTTOM,
	};
public:
	//ID3D11InputLayout*			m_pInputLayout;
	//ID3D11VertexShader*			m_pVertexShader;
	//ID3D11PixelShader*			m_pPixelShader;
	//ID3D11RasterizerState*		m_pRSSolidBack;
	//ID3D11RasterizerState*		m_pRSWireBack;
	//ID3D11RasterizerState*		m_pRS;
	//ID3D11ShaderResourceView*	m_pTextureSRV;
	//ID3D11SamplerState*			m_pWrapLinear;
public:
	mySquare(myVector3& vCenter, float& fRange)
	{
		m_VertexList.resize(6);
		m_IndexList.resize(6);
		m_IndexList[0] = 0;
		m_IndexList[1] = 1;
		m_IndexList[2] = 2;
		m_IndexList[3] = 2;
		m_IndexList[4] = 1;
		m_IndexList[5] = 3;
		m_fRange = fRange;
		m_vCenter = vCenter;
		m_VertexList[LEFT_TOP] =
		{
			myVector3(-fRange + vCenter.x, fRange + vCenter.y, vCenter.z),
			myVector3(0,0,0),
			myVector4(1,1,1,1),
			myVector2(0,0)
		};
		m_VertexList[RIGHT_TOP] =
		{
			myVector3(fRange + vCenter.x, fRange + vCenter.y, vCenter.z),
			myVector3(0,0,0),
			myVector4(1,1,1,1),
			myVector2(0,0)
		};
		m_VertexList[LEFT_BOTTOM] =
		{
			myVector3(-fRange + vCenter.x, -fRange + vCenter.y, vCenter.z),
			myVector3(0,0,0),
			myVector4(1,1,1,1),
			myVector2(0,0)
		};
		m_VertexList[RIGHT_BOTTOM] =
		{
			myVector3(fRange + vCenter.x, -fRange + vCenter.y, vCenter.z),
			myVector3(0,0,0),
			myVector4(1,1,1,1),
			myVector2(0,0)
		};
	}
	mySquare()
	{
		m_VertexList.resize(6);
		m_IndexList.resize(6);
		m_IndexList[0] = 0;
		m_IndexList[1] = 1;
		m_IndexList[2] = 2;
		m_IndexList[3] = 2;
		m_IndexList[4] = 1;
		m_IndexList[5] = 3;
		m_fRange = 0;
		m_vCenter = { 0,0,0 };
		m_VertexList[LEFT_TOP] =
		{
			myVector3(-1, 1, 0),
			myVector3(0,0,0),
			myVector4(1,1,1,1),
			myVector2(0,0)
		};
		m_VertexList[RIGHT_TOP] =
		{
			myVector3(1, 1, 0),
			myVector3(0,0,0),
			myVector4(1,1,1,1),
			myVector2(0,0)
		};
		m_VertexList[LEFT_BOTTOM] =
		{
			myVector3(-1, -1, 0),
			myVector3(0,0,0),
			myVector4(1,1,1,1),
			myVector2(0,0)
		};
		m_VertexList[RIGHT_BOTTOM] =
		{
			myVector3(1, -1, 0),
			myVector3(0,0,0),
			myVector4(1,1,1,1),
			myVector2(0,0)
		};
	}
};

class myBox : public myBaseFigure
{
public:
	enum
	{
		FRONT = 0,
		BACK,
		LEFT,
		RIGHT,
		UP,
		DOWN,
	};
	mySquare			m_sSquare[6];
	myVector3			m_vCenter;
	float				m_fWidth;
	vector<MY_VERTEX>	m_VertexList;
	vector<DWORD>		m_IndexList;
public:
	myBox(myVector3 vCenter, float fWidth)
	{

	}
};

