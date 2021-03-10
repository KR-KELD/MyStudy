#pragma once
#include "myBaseObject.h"
class myCollision
{
public:
	static bool InterSectSegToFace(Vector3& vSegStart, Vector3& vSegEnd, Vector3& vFaceNormal,
		Vector3& v1, Vector3& v2, Vector3& v3, Vector3* vRet);
	static bool DetermineFace(Vector3& vPos, Vector3& vFaceNormal,
		Vector3& v1, Vector3& v2, Vector3& v3);
	static bool InterSectSegToFace(MY_RAY& myRay,
		Vector3& v1, Vector3& v2, Vector3& v3,
		float* t, float* u, float* v);
	static bool InterSectRayToBox(MY_RAY& myRay, MY_BOX& myBox,Vector3* vRet);
	static bool InterSectRayToSphere(MY_RAY& myRay, MY_SPHERE& mySphere);
};


