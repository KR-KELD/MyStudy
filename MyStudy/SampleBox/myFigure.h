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
	//버텍스 버퍼
	myVector3			m_vCenter;
	float				m_fRange;
	ID3D11Buffer*		m_pVertexBuffer;
	ID3D11Buffer*		m_pIndexBuffer;
	vector<MY_VERTEX>	m_VertexList;
	vector<DWORD>		m_IndexList;
public:
	virtual bool Init(ID3D11Device* pDevice);
	virtual bool Frame();
	virtual bool Render();
	virtual bool Render(ID3D11DeviceContext* pDeviceContext);
	virtual bool Release();
public:

};

class mySquare : public myBaseFigure
{
public:
	virtual bool Release();
public:
	enum VertexRot
	{
		LEFT_TOP = 0,
		RIGHT_TOP,
		LEFT_BOTTOM,
		RIGHT_BOTTOM,
	};
	enum SquareRot
	{
		FRONT = 0,
		BACK,
		LEFT,
		RIGHT,
		UP,
		DOWN,
	};
public:
	bool SetSquareRot(myVector3& vCenter, SquareRot type);
public:
	mySquare(myVector3& vCenter, float& fRange)
	{
		m_VertexList.resize(4);
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
			myVector2(1,0)
		};
		m_VertexList[LEFT_BOTTOM] =
		{
			myVector3(-fRange + vCenter.x, -fRange + vCenter.y, vCenter.z),
			myVector3(0,0,0),
			myVector4(1,1,1,1),
			myVector2(0,1)
		};
		m_VertexList[RIGHT_BOTTOM] =
		{
			myVector3(fRange + vCenter.x, -fRange + vCenter.y, vCenter.z),
			myVector3(0,0,0),
			myVector4(1,1,1,1),
			myVector2(1,1)
		};
	}
	mySquare()
	{
		m_VertexList.resize(4);
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
			myVector2(1,0)
		};
		m_VertexList[LEFT_BOTTOM] =
		{
			myVector3(-1, -1, 0),
			myVector3(0,0,0),
			myVector4(1,1,1,1),
			myVector2(0,1)
		};
		m_VertexList[RIGHT_BOTTOM] =
		{
			myVector3(1, -1, 0),
			myVector3(0,0,0),
			myVector4(1,1,1,1),
			myVector2(1,1)
		};
	}
};

class myBox/* : public myBaseFigure*/
{
public:
	myVector3			m_vCenter;
	float				m_fRange;
	mySquare			m_sSquare[6];
public:
	myBox(myVector3 vCenter, float fRange)
	{
		m_vCenter = vCenter;
		m_fRange = fRange;
		for (int i = 0; i < 6; i++)
		{
			m_sSquare[i].m_fRange = fRange;
			m_sSquare[i].SetSquareRot(vCenter, (mySquare::SquareRot)i);
		}
	}
	myBox()
	{
		m_vCenter = myVector3{ 0.0f,0.0f ,0.0f };
		m_fRange = 1.0f;
		for (int i = 0; i < 6; i++)
		{
			m_sSquare[i].m_fRange = m_fRange;
			m_sSquare[i].SetSquareRot(m_vCenter, (mySquare::SquareRot)i);
		}
	}
	bool Init(ID3D11Device* pDevice);
	bool Render(ID3D11DeviceContext* pDeviceContext);
	bool Release();
};

