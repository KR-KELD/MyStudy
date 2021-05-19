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

struct MY_SPHERE
{
	Vector3		vCenter;
	float		fRadius;
};

struct MY_BOX
{
	// Common
	Vector3		vCenter;
	Vector3		vPos[8];
	// AABB
	Vector3		vMax;
	Vector3		vMin;
	// OBB
	Vector3		vAxis[3];
	float		fExtent[3];
	void SetBox(Vector3& min, Vector3 max)
	{
		vMax = max;
		vMin = min;
		vCenter = (vMax + vMin) / 2.0f;
		Vector3 range = vMax - vCenter;
		fExtent[0] = range.x;
		fExtent[1] = range.y;
		fExtent[2] = range.z;
		vPos[0] = Vector3(vMin.x,vMax.y, vMin.z);
		vPos[1] = Vector3(vMax.x,vMax.y, vMin.z);
		vPos[2] = Vector3(vMax.x,vMin.y, vMin.z);
		vPos[3] = Vector3(vMin.x,vMin.y, vMin.z);
		vPos[4] = Vector3(vMin.x,vMax.y, vMax.z);
		vPos[5] = Vector3(vMax.x,vMax.y, vMax.z);
		vPos[6] = Vector3(vMax.x,vMin.y, vMax.z);
		vPos[7] = Vector3(vMin.x,vMin.y, vMax.z);
	}

};

struct MY_PLANE
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