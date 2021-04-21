#include "myCollision.h"

bool myCollision::IntersectSphereToSphere(MY_SPHERE & mySp1, MY_SPHERE & mySp2)
{
	float fDist = (mySp1.vCenter - mySp2.vCenter).Length();
	if ((mySp1.fRadius + mySp2.fRadius) > fDist) return true;
	return false;
}

bool myCollision::IntersectSegToFace(Vector3 & vSegStart, Vector3 & vSegEnd, Vector3 & vFaceNormal,
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

bool myCollision::IntersectSegToFace(MY_RAY & myRay, Vector3 & v1, Vector3 & v2, Vector3 & v3,
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
bool myCollision::IntersectRayToBox(MY_RAY& myRay, MY_BOX& myBox, Vector3* vRet)
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
//수정
bool myCollision::IntersectRayToBox(MY_RAY & myRay, MY_BOX & myBox)
{

	float fDirDot[3];
	float fAbsDir[3];
	float fInner[3];
	float fAbsInner[3];
	Vector3 vOC = myRay.vOrigin - myBox.vCenter;
	fDirDot[0] = myRay.vDir.Dot(myBox.vAxis[0]);

	fInner[0] = vOC.Dot(myBox.vAxis[0]);
	fAbsInner[0] = fabs(fInner[0]);
	//박스 범위 안쪽 체크와 레이 방향체크를 동시에 한다
	if (fAbsInner[1] > myBox.fExtent[1] && fDirDot[1] * fInner[1] >= 0.0f)
		return false;
	fDirDot[1] = myRay.vDir.Dot(myBox.vAxis[1]);

	fInner[1] = vOC.Dot(myBox.vAxis[1]);
	fAbsInner[1] = fabs(fInner[1]);
	if (fAbsInner[1] > myBox.fExtent[1] && fDirDot[1] * fInner[1] >= 0.0f)
		return false;
	fDirDot[2] = myRay.vDir.Dot(myBox.vAxis[2]);

	fInner[2] = vOC.Dot(myBox.vAxis[2]);
	fAbsInner[2] = fabs(fInner[2]);
	if (fAbsInner[2] > myBox.fExtent[2] && fDirDot[2] * fInner[2] >= 0.0f)
		return false;

	fAbsDir[0] = fabs(fDirDot[0]);
	fAbsDir[1] = fabs(fDirDot[1]);
	fAbsDir[2] = fabs(fDirDot[2]);

	//레이디렉션과 obb큐브의 기저축과의 직교축을 분리축으로 하는 투영을 간략하게 정리 한 수식
	float fAbsDirCrossDot[3];
	float fRhs;
	Vector3 fDirCross;
	fDirCross = myRay.vDir.Cross(vOC);
	fAbsDirCrossDot[0] = fabs(fDirCross.Dot(myBox.vAxis[0]));
	fRhs = myBox.fExtent[1] * fAbsDir[2] + myBox.fExtent[2] * fAbsDir[1];
	if (fAbsDirCrossDot[0] > fRhs) return false;

	fAbsDirCrossDot[1] = fabs(fDirCross.Dot(myBox.vAxis[0]));
	fRhs = myBox.fExtent[0] * fAbsDir[2] + myBox.fExtent[2] * fAbsDir[0];
	if (fAbsDirCrossDot[1] > fRhs) return false;

	fAbsDirCrossDot[2] = fabs(fDirCross.Dot(myBox.vAxis[0]));
	fRhs = myBox.fExtent[0] * fAbsDir[1] + myBox.fExtent[1] * fAbsDir[0];
	if (fAbsDirCrossDot[2] > fRhs) return false;
	return true;
}

bool myCollision::IntersectRayToSphere(MY_RAY & myRay, MY_SPHERE & mySphere)
{
	Vector3 vDir = myRay.vOrigin - mySphere.vCenter;
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
