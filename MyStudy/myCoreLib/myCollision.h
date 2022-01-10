#pragma once
#include "myBaseObject.h"
class myCollision
{
public:
	//��ü�� ��ü�� �浹 �Ǻ�
	static bool IntersectSphereToSphere(MY_SPHERE& mySp1, MY_SPHERE& mySp2);
	//���а� Ʈ���̾ޱ��� �浹 �Ǻ�
	static bool IntersectSegToFace(Vector3& vSegStart, Vector3& vSegEnd, Vector3& vFaceNormal,
		Vector3& v1, Vector3& v2, Vector3& v3, Vector3* vRet);
	//���а� Ʈ���̾ޱ��� �浹 �Ǻ�(UV����)
	static bool IntersectSegToFace(MY_RAY& myRay,
		Vector3& v1, Vector3& v2, Vector3& v3,
		float* t, float* u, float* v);
	//Ʈ���̾ޱ� �ȿ� �� ���� �� �ִ��� �Ǻ�
	static bool DetermineFace(Vector3& vPos, Vector3& vFaceNormal,
		Vector3& v1, Vector3& v2, Vector3& v3);
	//���̿� �ڽ��� �浹 �Ǻ�
	static bool IntersectRayToBox(MY_RAY& myRay, MY_BOX& myBox, Vector3* vRet);
	static bool IntersectRayToBox(MY_RAY& myRay, MY_BOX& myBox);
	//���̿� ��ü�� �浹 �Ǻ�
	static bool IntersectRayToSphere(MY_RAY& myRay, MY_SPHERE& mySphere);
};


