#include "myFrustum.h"
bool myFrustum::Create(ID3D11DeviceContext* pContext)
{
	//����ü �������� ������Ʈ ����
	if (!m_FrustumObj.Create(pContext, L"vs.txt", L"ps.txt",L""))
	{
		return false;
	}
	//����ü ���������� ������ ��������
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

bool myFrustum::Frame(Matrix& ViewProjInv)
{
	////����İ� ��������� ������� �����
	//for (int iVertex = 0; iVertex < 24; iVertex++)
	//{
	//	//ndc������ �������� �����鿡 ��*��������� ������� ���ؼ� ���� ���� ������������ ������ش�
	//	Vector3& v = m_VertexList[iVertex].p;
	//	m_FrustumObj.m_VertexList[iVertex].p = Vector3::Transform(v, ViewProjInv);// *matInvViewProj;
	//}
	////��ȯ�� ���������� ������ ���� ���۸� �������ش�
	//m_pd3dContext->UpdateSubresource(
	//	m_FrustumObj.m_pVertexBuffer, 0, NULL,
	//	&m_FrustumObj.m_VertexList.at(0), 0, 0);

	//�������� ��� ����
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
	//�������� �� ���� �� �������� ��ġ���踦 �Ǻ�
	//����� �����Ŀ� ���� �����ؼ� ����� ����� ��
	//������ ����� ���� ���� �ȴ�
	//���ڵ�� ����� �븻�� ������ ���ؼ� �ٱ��ʿ� ���� ���� �Ÿ��°ŷ� ����
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

BOOL myFrustum::ClassifyBox(myShapeBox box)
{
	return 0;
}
