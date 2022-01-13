#pragma once
#include "myBaseObject.h"
class myCollision
{
public:
	//구체와 구체의 충돌 판별
	static bool IntersectSphereToSphere(MY_SPHERE& mySp1, MY_SPHERE& mySp2);
	//선분과 트라이앵글의 충돌 판별
	static bool IntersectSegToFace(Vector3& vSegStart, Vector3& vSegEnd, Vector3& vFaceNormal,
		Vector3& v1, Vector3& v2, Vector3& v3, Vector3* vRet);
	//선분과 트라이앵글의 충돌 판별(UV버전)
	static bool IntersectSegToFace(MY_RAY& myRay,
		Vector3& v1, Vector3& v2, Vector3& v3,
		float* t, float* u, float* v);
	//트라이앵글 안에 한 점이 들어가 있는지 판별
	static bool DetermineFace(Vector3& vPos, Vector3& vFaceNormal,
		Vector3& v1, Vector3& v2, Vector3& v3);
	//레이와 박스의 충돌 판별
	static bool IntersectRayToBox(MY_RAY& myRay, MY_BOX& myBox, Vector3* vRet);
	static bool IntersectRayToBox(MY_RAY& myRay, MY_BOX& myBox);
	//레이와 구체의 충돌 판별
	static bool IntersectRayToSphere(MY_RAY& myRay, MY_SPHERE& mySphere);
};


