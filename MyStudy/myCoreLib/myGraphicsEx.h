#pragma once
#include "myModelGraphics.h"
//보류
//트라이앵글리스트도 다 바꿔야하나
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

class myGraphicsEx : public myModelGraphics
{
public:
	DEFINE_COMPONENT(myGraphicsEx, myModelGraphics, true);
public:
	vector<PNCT2_VERTEX>	m_VertexList2;
public:
	virtual bool    CreateVertexBuffer() override;
	virtual bool	CreateInputLayout() override;
public:
	virtual void CreateTangent(PNCT2_VERTEX *v1, PNCT2_VERTEX *v2, PNCT2_VERTEX *v3,
		Vector3 *vTangent, Vector3 *vBiNormal, Vector3 *vNormal);
	virtual bool	MultiDraw(ID3D11DeviceContext*	pd3dContext) override;
	//virtual void SetTangent();
public:
	myGraphicsEx();
	virtual ~myGraphicsEx();
};

