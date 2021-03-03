#pragma once
#include "myShape.h"

//카메라 게임오브젝트
//아래 카메라 컴포넌트
//프러스텀 박스도 컴포넌트화 해야할것같음
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
	virtual bool Frame(Matrix& matInvViewProj);
	virtual bool Release();
	BOOL	ClassifyPoint(Vector3 v);
	BOOL	ClassifyBox(myShapeBox box);
	BOOL    CheckOBBInPlane(MY_BOX* pBox);
};

