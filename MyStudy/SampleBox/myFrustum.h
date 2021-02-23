#pragma once
#include "myShape.h"

struct myPlane
{
	//����� ������
	//a,b,c�� ����� �븻���� x,y,z
	//d�� ������ ��� ������ �Ÿ�
	float a, b, c, d;
	//�� 3���� ����� �����ϴ� ���
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
	//�븻���� 1���� ����� �� 1��
	void Create(Vector3 n, Vector3 v)
	{
		n.Normalize();
		a = n.x;
		b = n.y;
		c = n.z;
		d = -n.Dot(v);
	}
};
//ī�޶� ���ӿ�����Ʈ
//�Ʒ� ī�޶� ������Ʈ
//�������� �ڽ��� ������Ʈȭ �ؾ��ҰͰ���
class myFrustum
{
public:
	//�������� �ڽ�
	myShapeBox  m_FrustumObj;
	//�������� �ڽ��� ���ؽ� ������
	std::vector<PNCT_VERTEX>	m_VertexList;
	//�������� ��� 6��
	std::vector<myPlane>   m_Plane;
public:
	virtual bool Create(ID3D11Device* pd3dDevice);
	virtual bool Frame();

	BOOL	ClassifyPoint(Vector3 v);
	BOOL	ClassifyBox(myShapeBox box);
};

