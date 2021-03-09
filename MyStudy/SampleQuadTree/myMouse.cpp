#include "myMouse.h"

void myMouse::ScreenToRay()
{
	POINT pos = g_Input.GetMouse();
	Matrix proj = g_pMainCamTransform->m_matProj;
	Vector3 view;
	view.x = (((2.0f * pos.x) / g_rtClient.right) - 1) / proj._11;
	view.y = -(((2.0f * pos.y) / g_rtClient.bottom) - 1) / proj._22;
	view.z = 1.0f;

	Matrix invView = g_pMainCamTransform->m_matView.Invert();
	m_myRay.m_vOrigin = Vector3::Transform(m_myRay.m_vOrigin, invView);
	m_myRay.m_vDir = view;
	m_myRay.m_vDir = Vector3::TransformNormal(m_myRay.m_vDir, invView);
	m_myRay.m_vDir.Normalize();
}

void myMouse::MousePicking(myMap* pMap)
{
	ScreenToRay();
	Vector3 v[3];
	for (int face = 0; face < pMap->m_IndexList.size() / 3; face++)
	{
		v[0] = pMap->m_VertexList[pMap->m_IndexList[face * 3 + 0]].p;
		v[1] = pMap->m_VertexList[pMap->m_IndexList[face * 3 + 1]].p;
		v[2] = pMap->m_VertexList[pMap->m_IndexList[face * 3 + 2]].p;

		Vector3 vEnd = m_myRay.m_vOrigin + m_myRay.m_vDir * m_fRange;
		Vector3 vFaceNormal = (v[1] - v[0]).Cross(v[2] - v[0]);
		vFaceNormal.Normalize();
		if (InterSection(m_myRay.m_vOrigin, vEnd, vFaceNormal, v[0], v[1], v[2]))
		{
			return;
		}
	}
}

bool myMouse::InterSection(Vector3& vSegStart, Vector3& vSegEnd, Vector3& vFaceNormal,
	Vector3& v1, Vector3& v2, Vector3& v3)
{
	Vector3 vDir = vSegEnd - vSegStart;
	float fDir = vFaceNormal.Dot(vDir);
	float fPlane = vFaceNormal.Dot(v1 - vSegStart);
	float fRatio = fPlane / fDir;
	//´Ù½Ã
	if (fRatio < 0.0f || fRatio > 1.0f)
	{
		return false;
	}
	m_vIntersectionPos = vSegStart + vDir * fRatio;
	if (!DetermineFace(m_vIntersectionPos, vFaceNormal, v1, v2, v3))
	{
		m_vIntersectionPos = Vector3::Zero;
		return false;
	}
	return true;
}

bool myMouse::DetermineFace(Vector3& vPos, Vector3& vFaceNormal, 
	Vector3& v1, Vector3& v2, Vector3& v3)
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

bool myMouse::InterSectDetermine(Vector3 & vRayStart, Vector3 & vRayDir,
	Vector3& v1, Vector3& v2, Vector3& v3,
		float * t, float * u, float * v)
{
	Vector3 vEdge1 = v2 - v1;
	Vector3 vEdge2 = v3 - v1;
	Vector3 vP = vRayDir.Cross(vEdge2);
	float fD = vEdge1.Dot(vP);
	Vector3 vT;
	if (fD > 0)
	{
		vT = vRayStart - v1;
	}
	else
	{
		vT = v1 - vRayStart;
		fD = -fD;
	}
	if (fD < EPSILON) return false;

	*u = vT.Dot(vP);
	if (*u < 0.0f || *u > fD) return false;

	Vector3 vQ = vT.Cross(vEdge1);
	*v = vRayDir.Dot(vQ);
	if (*v < 0.0f || *u + *v > fD) return false;

	*t = vEdge2.Dot(vQ);
	float fInvD = 1.0f / fD;

	*t *= fInvD;
	*u *= fInvD;
	*v *= fInvD;
	return true;
}

myMouse::myMouse()
{
	m_fRange = 1000.0f;
}

myMouse::~myMouse()
{
}
