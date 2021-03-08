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
	Vector3 v[3];
	for (int face = 0; face < pMap->m_IndexList.size() / 3; face++)
	{
		v[0] = pMap->m_VertexList[pMap->m_IndexList[face * 3 + 0]].p;
		v[1] = pMap->m_VertexList[pMap->m_IndexList[face * 3 + 1]].p;
		v[2] = pMap->m_VertexList[pMap->m_IndexList[face * 3 + 2]].p;

		Vector3 end = m_myRay.m_vOrigin + m_myRay.m_vDir * m_fRange;
		Vector3 faceNormal = (v[1] - v[0]).Cross(v[2] - v[0]);
		faceNormal.Normalize();

	}
}

bool myMouse::InterSection(Vector3 vSegStart, Vector3 vSegEnd, Vector3 vFaceNormal,
	Vector3 vFaceV1, Vector3 vFaceV2, Vector3 vFaceV3)
{
	Vector3 vDir = vSegEnd - vSegStart;
	float fDir = vFaceNormal.Dot(vSegEnd - vSegStart);
	float fPlane = vFaceNormal.Dot(vFaceV1 - vSegStart);
	float fRatio = fPlane / fDir;
	//�ٽ�
	if (fRatio < 0.0f || fRatio > 1.0f)
	{
		return false;
	}
	m_vIntersectionPos = vSegStart + vDir * fRatio;
	return true;
}

bool myMouse::DiscribeFace(Vector3 vPos, Vector3 vFaceNormal, Vector3 vFaceV1,
	Vector3 vFaceV2, Vector3 vFaceV3)
{
	Vector3 vEdge1 = vFaceV2 - vFaceV1;
	Vector3 vEdge2 = vFaceV3 - vFaceV1;
	Vector3	vPos1 = vPos - vFaceV1;
	Vector3 vNormal1 = vEdge1.Cross(vPos1);
	vNormal1.Normalize();
	float fD = vFaceNormal.Dot(vNormal1);
	if (fD < 0.0f) return false;
	vNormal1 = vPos1.Cross(vEdge2);
	vNormal1.Normalize();
	fD = vFaceNormal.Dot(vNormal1);
	if (fD < 0.0f) return false;

	vEdge1 = vFaceV3 - vFaceV2;
	vEdge2 = vFaceV1 - vFaceV2;
	vPos1 = vPos - vFaceV2;
	vNormal1 = vEdge1.Cross(vPos1);
	vNormal1.Normalize();
	float fD = vFaceNormal.Dot(vNormal1);
	if (fD < 0.0f) return false;
	vNormal1 = vPos1.Cross(vEdge2);
	vNormal1.Normalize();
	fD = vFaceNormal.Dot(vNormal1);
	if (fD < 0.0f) return false;

	return true;
}

myMouse::myMouse()
{
	m_fRange = 1000.0f;
}

myMouse::~myMouse()
{
}
