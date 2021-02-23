#pragma once
#include "myShape.h"

struct myPlane
{
	//평면의 방정식
	//a,b,c는 평면의 노말벡터 x,y,z
	//d는 원점과 평면 사이의 거리
	float a, b, c, d;
	//점 3개로 평면을 구성하는 방법
	void Create(Vector3 v0, Vector3 v1, Vector3 v2)
	{
		Vector3 n;
		Vector3 e0 = v1 - v0;
		Vector3 e1 = v2 - v0;
		n = e0.Cross(e1);
		n.Normalize();
		a = n.x;
		b = n.y;
		c = n.z;
		d = -n.Dot(v0);
	}
	//노말벡터 1개와 평면의 점 1개
	void Create(Vector3 n, Vector3 v)
	{
		n.Normalize();
		a = n.x;
		b = n.y;
		c = n.z;
		d = -n.Dot(v);
	}
};
//카메라 게임오브젝트
//아래 카메라 컴포넌트
//프러스텀 박스도 컴포넌트화 해야할것같음
class myFrustum
{
public:
	//프러스텀 박스
	myShapeBox  m_FrustumObj;
	//프러스텀 박스의 버텍스 데이터
	std::vector<PNCT_VERTEX>	m_VertexList;
	//프러스텀 평면 6개
	std::vector<myPlane>   m_Plane;
public:
	virtual bool Create(ID3D11Device* pd3dDevice);
	virtual bool Frame();

	BOOL	ClassifyPoint(Vector3 v);
	BOOL	ClassifyBox(myShapeBox box);
};

