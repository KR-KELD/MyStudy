#pragma once
#include "myShape.h"

//ī�޶� ���ӿ�����Ʈ
//�Ʒ� ī�޶� ������Ʈ
//�������� �ڽ��� ������Ʈȭ �ؾ��ҰͰ���
class myFrustum
{
public:
	//�������� �ڽ�
	myShapeBox					m_FrustumObj;
	//�������� �ڽ��� ���ؽ� ������
	std::vector<PNCT_VERTEX>	m_VertexList;
	//�������� ��� 6��
	std::vector<MY_PLANE>		m_Plane;
public:
	virtual bool Create();
	virtual bool Frame(Matrix& matInvViewProj);
	virtual bool Release();
	BOOL	ClassifyPoint(Vector3 v);
	BOOL	ClassifyBox(myShapeBox box);
	BOOL    CheckOBBInPlane(MY_BOX* pBox);
};

