#pragma once
#include "myGraphics.h"
//º¸·ù
struct PNCT2_VERTEX
{
	Vector3 p;
	Vector3 n;
	Vector4 c;
	Vector2 t;
	Vector3 tangent;
	bool operator == (const PNCT2_VERTEX & Vertex)
	{
		if (p == Vertex.p  && n == Vertex.n && 	c == Vertex.c  &&	t == Vertex.t && tangent == Vertex.tangent)
		{
			return true;
		}
		return  false;
	}
	PNCT2_VERTEX() {}
	PNCT2_VERTEX(
		Vector3 vp,
		Vector3 vn,
		Vector4 vc,
		Vector2 vt,
		Vector3 vTangent)
	{
		p = vp, n = vn, c = vc, t = vt, tangent = vTangent;
	}
};

class myGraphicsEx : public myGraphics
{
public:
	DEFINE_COMPONENT(myGraphicsEx, myGraphics, true);
public:
	vector<PNCT2_VERTEX>	m_VertexList2;
public:
	virtual bool    CreateVertexBuffer() override;
	virtual bool	CreateInputLayout() override;
public:
	myGraphicsEx();
	virtual ~myGraphicsEx();
};

