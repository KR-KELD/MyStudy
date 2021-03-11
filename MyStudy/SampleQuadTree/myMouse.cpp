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
	m_myRay.vOrigin = Vector3::Zero;
	m_myRay.vOrigin = Vector3::Transform(m_myRay.vOrigin, invView);
	m_myRay.vDir = view;
	m_myRay.vDir = Vector3::TransformNormal(m_myRay.vDir, invView);
	m_myRay.vDir.Normalize();
}

bool myMouse::PickingFace(myMap* pMap, myNode* pNode)
{
	ScreenToRay();
	Vector3 v[3];
	float fT, fU, fV;
	for (int face = 0; face < pNode->m_IndexList.size() / 3; face++)
	{
		v[0] = pMap->m_VertexList[pNode->m_IndexList[face * 3 + 0]].p;
		v[1] = pMap->m_VertexList[pNode->m_IndexList[face * 3 + 1]].p;
		v[2] = pMap->m_VertexList[pNode->m_IndexList[face * 3 + 2]].p;

		Vector3 vEnd = m_myRay.vOrigin + m_myRay.vDir * m_fRange;
		Vector3 vFaceNormal = (v[1] - v[0]).Cross(v[2] - v[0]);
		vFaceNormal.Normalize();
		if (myCollision::InterSectSegToFace(m_myRay, v[0], v[1], v[2],&fT,&fU,&fV))
		{
			m_vIntersectionPos = m_myRay.vOrigin + m_myRay.vDir * fT;
			return true;
		}
	}
	return false;
}

bool myMouse::PickingAABBBox(myNode * pNode)
{
	ScreenToRay();
	if (myCollision::InterSectRayToBox(m_myRay, pNode->m_myBox, &m_vIntersectionPos))
	{
		return true;
	}
	return false;
}

myMouse::myMouse()
{
	m_fRange = 1000.0f;
}

myMouse::~myMouse()
{
}
