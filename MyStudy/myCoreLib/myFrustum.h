#pragma once
#include "myShape.h"

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
	//�������� ����
	virtual bool Frame(Matrix& matInvViewProj);\
	virtual bool Release();
	//��-����������� �浹 �Ǻ�
	BOOL	ClassifyPoint(Vector3 v);
	//�ڽ�-����������� �浹 �Ǻ�
	BOOL    CheckOBBInPlane(MY_BOX* pBox);

	//bool	ClassifyLine(Vector3 v);
	//BOOL	ClassifyBox(myShapeBox box);
};

