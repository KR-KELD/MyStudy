#pragma once
#include "myStd.h"

class myBaseObject
{
public:
	
};

enum myObjectType
{
	OBJECT_OBJECT = 0,
	OBJECT_PLAYER,
	OBJECT_ENEMY,
	OBJECT_MAP,
	OBJECT_UI,

};

enum myCollisionState
{
	COLLISION_NONE = 0,
	COLLISION_BEGIN,
	COLLISION_STAY,
	COLLISION_END,

};

enum myOverlapState
{
	OVERLAP_DEFAULT = 0,
	OVERLAP_HOVER,
	OVERLAP_FOCUS,
	OVERLAP_ACTIVE,
	OVERLAP_SELECTED,

};

struct MY_SPHERE
{
	Vector3		vCenter;
	float		fRadius;
};

struct MY_BOX
{
	Vector3		vCenter;
	Vector3		vPos[8];
	Vector3		vMax;
	Vector3		vMin;
	Vector3		vAxis[3];
	float		fExtent[3];
};

struct MY_PLANE
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

struct MY_RAY
{
	Vector3 vOrigin;
	Vector3 vDir;
	MY_RAY()
	{
		vOrigin = Vector3::Zero;
		vDir = Vector3(0.0f, 0.0f, 1.0f);
	}
};