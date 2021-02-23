#include "myFrustum.h"

bool myFrustum::Create(ID3D11Device* pd3dDevice)
{
	//육면체 프러스텀 오브젝트 생성
	if (!m_FrustumObj.Create(pd3dDevice, L"vs.txt",
		L"ps.txt",
		L""))
	{
		return false;
	}
	//육면체 프러스텀의 정보를 가져오기
	for (auto v : m_FrustumObj.m_VertexList)
	{
		m_VertexList.push_back(v);
	}
	//???
	//m_VertexList[0].p = Vector3(-1.0f, 1.0f, 0.0f);
	//m_VertexList[1].p = Vector3(1.0f, 1.0f, 0.0f);
	//m_VertexList[2].p = Vector3(1.0f, -1.0f, 0.0f);
	//m_VertexList[3].p = Vector3(-1.0f, -1.0f, 0.0f);

	//m_VertexList[8].p = Vector3(1.0f, 1.0f, 0.0f);
	//m_VertexList[11].p = Vector3(1.0f, -1.0f, 0.0f);

	//m_VertexList[13].p = Vector3(-1.0f, 1.0f, 0.0f);
	//m_VertexList[14].p = Vector3(-1.0f, -1.0f, 0.0f);

	//m_VertexList[18].p = Vector3(1.0f, 1.0f, 0.0f);
	//m_VertexList[19].p = Vector3(-1.0f, 1.0f, 0.0f);


	//m_VertexList[20].p = Vector3(-1.0f, -1.0f, 0.0f);
	//m_VertexList[21].p = Vector3(1.0f, -1.0f, 0.0f);

	m_Plane.resize(6);
	return true;
}

bool myFrustum::Frame()
{
	//프러스텀 평면 구성
	if (m_Plane.size() <= 0) return true;
	m_Plane[0].Create(
		m_FrustumObj.m_VertexList[0].p,
		m_FrustumObj.m_VertexList[1].p,
		m_FrustumObj.m_VertexList[2].p);
	m_Plane[1].Create(
		m_FrustumObj.m_VertexList[4].p,
		m_FrustumObj.m_VertexList[5].p,
		m_FrustumObj.m_VertexList[6].p);
	m_Plane[2].Create(
		m_FrustumObj.m_VertexList[8].p,
		m_FrustumObj.m_VertexList[9].p,
		m_FrustumObj.m_VertexList[10].p);
	m_Plane[3].Create(
		m_FrustumObj.m_VertexList[12].p,
		m_FrustumObj.m_VertexList[13].p,
		m_FrustumObj.m_VertexList[14].p);
	m_Plane[4].Create(
		m_FrustumObj.m_VertexList[16].p,
		m_FrustumObj.m_VertexList[17].p,
		m_FrustumObj.m_VertexList[18].p);
	m_Plane[5].Create(
		m_FrustumObj.m_VertexList[20].p,
		m_FrustumObj.m_VertexList[21].p,
		m_FrustumObj.m_VertexList[22].p);
	return true;
}

BOOL myFrustum::ClassifyPoint(Vector3 v)
{
	//프러스텀 각 평면과 한 점사이의 위치관계를 판별
	//평면의 방정식에 점을 대입해서 양수면 평면의 앞
	//음수면 평면의 뒤쪽 점이 된다
	//쌤코드는 평면의 노말이 안쪽을 향해서 바깥쪽에 찍힌 점을 거르는거로 추정
	for (int iPlane = 0; iPlane < m_Plane.size(); iPlane++)
	{
		float fDist = m_Plane[iPlane].a * v.x +
			m_Plane[iPlane].b * v.y +
			m_Plane[iPlane].c * v.z +
			m_Plane[iPlane].d;
		if (fDist > 0.0f)
		{
			return false;
		}
	}
	return true;
}

BOOL myFrustum::ClassifyBox(myShapeBox box)
{
	return 0;
}
