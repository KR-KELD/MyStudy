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

		Vector3 end = m_myRay.m_vOrigin + m_myRay.m_vDir * 1000.0f;
		Vector3 faceNormal = (v[1] - v[0]).Cross(v[2] - v[0]);
		faceNormal.Normalize();

	}
}

bool myMouse::InterSection()
{
}

bool myMouse::DiscribeFace()
{
}

myMouse::myMouse()
{
}

myMouse::~myMouse()
{
}
