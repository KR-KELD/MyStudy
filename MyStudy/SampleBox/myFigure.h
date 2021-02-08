#pragma once
#include "myMatrix.h"
#include "WICTextureLoader.h"

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

class myBox : public myBaseFigure
{
public:
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
	myVector3			m_vCenter;
	float				m_fRange;
public:

	myBox(myVector3 vCenter, float fRange)
	{
		m_vCenter = vCenter;
		m_fRange = fRange;
		m_VertexList.resize(24);
		m_IndexList.resize(36);
		for (int i = 0; i < 6; i++)
		{
			m_IndexList[0 + i * 6] = 0 + i * 4;
			m_IndexList[1 + i * 6] = 1 + i * 4;
			m_IndexList[2 + i * 6] = 2 + i * 4;
			m_IndexList[3 + i * 6] = 2 + i * 4;
			m_IndexList[4 + i * 6] = 1 + i * 4;
			m_IndexList[5 + i * 6] = 3 + i * 4;
		}
		SetBox(vCenter);
	}
	myBox()
	{
		m_vCenter = myVector3(0.0f, 0.0f, 0.0f);
		m_fRange = 1.0f;
		m_VertexList.resize(24);
		m_IndexList.resize(36);
		for (int i = 0; i < 6; i++)
		{
			m_IndexList[0 + i * 6] = 0 + i * 4;
			m_IndexList[1 + i * 6] = 1 + i * 4;
			m_IndexList[2 + i * 6] = 2 + i * 4;
			m_IndexList[3 + i * 6] = 2 + i * 4;
			m_IndexList[4 + i * 6] = 1 + i * 4;
			m_IndexList[5 + i * 6] = 3 + i * 4;
		}
		SetBox(m_vCenter);
	}
	bool SetBox(myVector3 & vCenter);
};

