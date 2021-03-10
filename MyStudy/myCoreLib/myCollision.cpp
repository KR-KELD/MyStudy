#include "myCollision.h"

bool myCollision::InterSectSegToFace(Vector3 & vSegStart, Vector3 & vSegEnd, Vector3 & vFaceNormal,
	Vector3 & v1, Vector3 & v2, Vector3 & v3, Vector3* vRet)
{
	Vector3 vDir = vSegEnd - vSegStart;
	float fDir = vFaceNormal.Dot(vDir);
	float fPlane = vFaceNormal.Dot(v1 - vSegStart);
	float fRatio = fPlane / fDir;
	//다시
	if (fRatio < 0.0f || fRatio > 1.0f)
	{
		return false;
	}
	*vRet = vSegStart + vDir * fRatio;
	return true;
}

bool myCollision::DetermineFace(Vector3 & vPos, Vector3 & vFaceNormal, Vector3 & v1, Vector3 & v2, Vector3 & v3)
{
	Vector3 vEdge1 = v2 - v1;
	Vector3 vEdge2 = v3 - v1;
	Vector3	vPos1 = vPos - v1;

	Vector3 vNormal1 = vEdge1.Cross(vPos1);
	vNormal1.Normalize();
	float fD = vFaceNormal.Dot(vNormal1);
	if (fD < 0.0f) return false;

	vNormal1 = vPos1.Cross(vEdge2);
	vNormal1.Normalize();
	fD = vFaceNormal.Dot(vNormal1);
	if (fD < 0.0f) return false;

	vEdge1 = v3 - v2;
	vPos1 = vPos - v2;
	vNormal1 = vEdge1.Cross(vPos1);
	vNormal1.Normalize();
	fD = vFaceNormal.Dot(vNormal1);
	if (fD < 0.0f) return false;

	return true;
}

bool myCollision::InterSectSegToFace(MY_RAY & myRay, Vector3 & v1, Vector3 & v2, Vector3 & v3,
	float * t, float * u, float * v)
{
	Vector3 vEdge1 = v2 - v1;
	Vector3 vEdge2 = v3 - v1;
	Vector3 vP = myRay.vDir.Cross(vEdge2);
	float fD = vEdge1.Dot(vP);
	Vector3 vT;
	if (fD > 0)
	{
		vT = myRay.vOrigin - v1;
	}
	else
	{
		vT = v1 - myRay.vOrigin;
		fD = -fD;
	}
	if (fD < EPSILON) return false;

	*u = vT.Dot(vP);
	if (*u < 0.0f || *u > fD) return false;

	Vector3 vQ = vT.Cross(vEdge1);
	*v = myRay.vDir.Dot(vQ);
	if (*v < 0.0f || *u + *v > fD) return false;

	*t = vEdge2.Dot(vQ);
	float fInvD = 1.0f / fD;

	*t *= fInvD;
	*u *= fInvD;
	*v *= fInvD;
	return true;
}
//aabb
bool myCollision::InterSectRayToBox(MY_RAY& myRay, MY_BOX& myBox, Vector3* vRet)
{
	Vector3 vMin;
	vMin.x = (myBox.vMin.x - myRay.vOrigin.x) / myRay.vDir.x + EPSILON;
	vMin.y = (myBox.vMin.y - myRay.vOrigin.y) / myRay.vDir.y + EPSILON;
	vMin.z = (myBox.vMin.z - myRay.vOrigin.z) / myRay.vDir.z + EPSILON;

	Vector3 vMax;
	vMax.x = (myBox.vMax.x - myRay.vOrigin.x) / myRay.vDir.x + EPSILON;
	vMax.y = (myBox.vMax.y - myRay.vOrigin.y) / myRay.vDir.y + EPSILON;
	vMax.z = (myBox.vMax.z - myRay.vOrigin.z) / myRay.vDir.z + EPSILON;
	//판별
	Vector3 vNearMin;
	vNearMin.x = min(vMin.x, vMax.x);
	vNearMin.y = min(vMin.y, vMax.y);
	vNearMin.z = min(vMin.z, vMax.z);
	Vector3 vFarMax;
	vFarMax.x = max(vMin.x, vMax.x);
	vFarMax.y = max(vMin.y, vMax.y);
	vFarMax.z = max(vMin.z, vMax.z);

	float minmax = min(min(vFarMax.x, vFarMax.y), vFarMax.z);
	float maxmin = max(max(vNearMin.x, vNearMin.y), vNearMin.z);

	if (minmax >= maxmin)
	{
		*vRet = myRay.vOrigin + myRay.vDir * maxmin;
		return true;
	}
	return false;
}

bool myCollision::InterSectRayToSphere(MY_RAY & myRay, MY_SPHERE & mySphere)
{
	Vector3 vDir = myRay.vOrigin- mySphere.vCenter;
	// B = 2(u dot (p0 - c ))
	float  fProj = myRay.vDir.Dot(vDir);
	// 제한된 길이(세그먼트) 제외처리
	//if (pRay->fExtent > -1.0f 
	// && pRay->fExtent < fabs(fProj)) return false;

	float b = 2.0f * fProj;
	// C= (p0-c) dot (p0-c) - r*r
	float c = vDir.Dot(vDir) - (mySphere.fRadius * mySphere.fRadius);
	// D = B*B - 4*AC;
	float fDiscriminant = (b*b) - (4.0f*c);
	if (fDiscriminant < 0.0f)
	{
		return false;
	}
	fDiscriminant = sqrtf(fDiscriminant);
	float t0 = (-b + fDiscriminant) / 2.0f;
	float t1 = (-b - fDiscriminant) / 2.0f;

	if (t0 >= 0.0f)
	{
		//m_vIntersection = pRay->vOrigin + pRay->vDirection * t0;
		return true;
	}
	if (t1 >= 0.0f)
	{
		//m_vIntersection = pRay->vOrigin + pRay->vDirection * t1;
		return true;
	}
	return false;
}
