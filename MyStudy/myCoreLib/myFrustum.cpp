#include "myFrustum.h"

BOOL myFrustum::CheckOBBInPlane(MY_BOX*  pBox)
{
	float fPlaneToCenter = 0.0f;
	float fDistance = 0.0f;
	Vector3 vDir;
	//각 평면과 박스의 충돌을 체크(분리축 검사)
	for (int iPlane = 0; iPlane < 6; iPlane++)
	{
		vDir = pBox->vAxis[0] * pBox->fExtent[0];
		fDistance = fabs(m_Plane[iPlane].a * vDir.x +
						 m_Plane[iPlane].b * vDir.y +
						 m_Plane[iPlane].c * vDir.z);
		vDir = pBox->vAxis[1] * pBox->fExtent[1];
		fDistance += fabs(m_Plane[iPlane].a * vDir.x +
						  m_Plane[iPlane].b * vDir.y + 
						  m_Plane[iPlane].c * vDir.z);
		vDir = pBox->vAxis[2] * pBox->fExtent[2];
		fDistance += fabs(m_Plane[iPlane].a * vDir.x + 
						  m_Plane[iPlane].b * vDir.y +
						  m_Plane[iPlane].c * vDir.z);

		fPlaneToCenter = m_Plane[iPlane].a * pBox->vCenter.x +
						 m_Plane[iPlane].b * pBox->vCenter.y +
						 m_Plane[iPlane].c * pBox->vCenter.z + 
						 m_Plane[iPlane].d;

		if (fPlaneToCenter <= -fDistance)
		{
			return FALSE;
		}
	}
	return TRUE;
}

bool myFrustum::Create()
{
	//육면체 프러스텀 오브젝트 생성
	if (!m_FrustumObj.Create(L"BasisVS.txt", L"BasisPS.txt",L""))
	{
		return false;
	}
	//육면체 프러스텀의 정보를 가져오기
	for (auto v : m_FrustumObj.m_VertexList)
	{
		m_VertexList.push_back(v);
	}
	m_VertexList[0].p.z = 0.0f;
	m_VertexList[1].p.z =  0.0f;
	m_VertexList[2].p.z =  0.0f;
	m_VertexList[3].p.z =  0.0f;

	m_VertexList[9].p.z =  0.0f;
	m_VertexList[11].p.z =  0.0f;

	m_VertexList[12].p.z =  0.0f;
	m_VertexList[14].p.z =  0.0f;

	m_VertexList[18].p.z =  0.0f;
	m_VertexList[19].p.z =  0.0f;

	m_VertexList[20].p.z =  0.0f;
	m_VertexList[21].p.z =  0.0f;

	m_Plane.resize(6);
	return true;
}

bool myFrustum::Frame(Matrix& matInvViewProj)
{
	for (int iVertex = 0; iVertex < 24; iVertex++)
	{
		//ndc공간의 프러스텀 정점들에 뷰*투영행렬의 역행렬을 곱해서 월드 공간 프러스텀으로 만들어준다
		Vector3& v = m_VertexList[iVertex].p;
		m_FrustumObj.m_VertexList[iVertex].p = Vector3::Transform(v, matInvViewProj);// *matInvViewProj;
	}
	//변환된 프러스텀의 정점과 정점 버퍼를 세팅해준다
	g_pImmediateContext->UpdateSubresource(
		m_FrustumObj.m_pVertexBuffer.Get(), 0, NULL,
		&m_FrustumObj.m_VertexList.at(0), 0, 0);

	//프러스텀 평면 구성
	if (m_Plane.size() <= 0) return true;
	m_Plane[0].Create(
		m_FrustumObj.m_VertexList[0].p,
		m_FrustumObj.m_VertexList[2].p,
		m_FrustumObj.m_VertexList[1].p);
	m_Plane[1].Create(
		m_FrustumObj.m_VertexList[4].p,
		m_FrustumObj.m_VertexList[6].p,
		m_FrustumObj.m_VertexList[5].p);
	m_Plane[2].Create(
		m_FrustumObj.m_VertexList[8].p,
		m_FrustumObj.m_VertexList[10].p,
		m_FrustumObj.m_VertexList[9].p);
	m_Plane[3].Create(
		m_FrustumObj.m_VertexList[12].p,
		m_FrustumObj.m_VertexList[14].p,
		m_FrustumObj.m_VertexList[13].p);
	m_Plane[4].Create(
		m_FrustumObj.m_VertexList[16].p,
		m_FrustumObj.m_VertexList[18].p,
		m_FrustumObj.m_VertexList[17].p);
	m_Plane[5].Create(
		m_FrustumObj.m_VertexList[20].p,
		m_FrustumObj.m_VertexList[22].p,
		m_FrustumObj.m_VertexList[21].p);
	return true;
}

bool myFrustum::Release()
{
	m_FrustumObj.Release();
	return true;
}

BOOL myFrustum::ClassifyPoint(Vector3 v)
{
	//프러스텀 각 평면과 한 점사이의 위치관계를 판별
	//평면의 방정식에 점을 대입해서 양수면 평면의 앞
	//음수면 평면의 뒤쪽 점이 된다
	for (int iPlane = 0; iPlane < m_Plane.size(); iPlane++)
	{
		float fDist = m_Plane[iPlane].a * v.x +
			m_Plane[iPlane].b * v.y +
			m_Plane[iPlane].c * v.z +
			m_Plane[iPlane].d;
		if (fDist < 0.0f)
		{
			return false;
		}
	}
	return true;
}

//bool myFrustum::ClassifyLine(Vector3 v)
//{
//	for (int iPlane = 0; iPlane < m_Plane.size(); iPlane++)
//	{
//		float fDist = m_Plane[iPlane].a * v.x +
//			m_Plane[iPlane].b * v.y +
//			m_Plane[iPlane].c * v.z +
//			m_Plane[iPlane].d;
//		if (fDist < 0.0f)
//		{
//			return false;
//		}
//	}
//	return true;
//}

//BOOL myFrustum::ClassifyBox(myShapeBox box)
//{
//	return 0;
//}
