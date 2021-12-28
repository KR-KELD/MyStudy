#pragma once
#include "myShape.h"

class myFrustum
{
public:
	//프러스텀 박스
	myShapeBox					m_FrustumObj;
	//프러스텀 박스의 버텍스 데이터
	std::vector<PNCT_VERTEX>	m_VertexList;
	//프러스텀 평면 6개
	std::vector<MY_PLANE>		m_Plane;
public:
	virtual bool Create();
	//프러스텀 갱신
	virtual bool Frame(Matrix& matInvViewProj);\
	virtual bool Release();
	//점-프러스텀평면 충돌 판별
	BOOL	ClassifyPoint(Vector3 v);
	//박스-프러스텀평면 충돌 판별
	BOOL    CheckOBBInPlane(MY_BOX* pBox);

	//bool	ClassifyLine(Vector3 v);
	//BOOL	ClassifyBox(myShapeBox box);
};

