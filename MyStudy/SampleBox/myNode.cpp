//#include "myNode.h"
//MY_BoxObject::MY_BoxObject()
//{
//	m_myBox.vCenter = Vector3(0.0f, 0.0f, 0.0f);
//	m_myBox.vMax = Vector3(1.0f, 1.0f, 1.0f);
//	m_myBox.vMin = Vector3(-1.0f, -1.0f, -1.0f);
//
//	m_vPosition = Vector3(31 - rand() % 62, 0, 31 - rand() % 62);
//	m_vColor = Vector4((rand() % 256) / 255.0f, (rand() % 256) / 255.0f, (rand() % 256) / 255.0f, 1.0f);
//
//	// g_matWorld Matrix = S * R
//	Matrix matScale, matRotation, matWorld;
//	matScale._11 = (rand() % 256) / 255.0f * 2.0f;
//	matScale._22 = (rand() % 256) / 255.0f * 2.0f;
//	matScale._33 = (rand() % 256) / 255.0f * 2.0f;
//	matRotation = Matrix::CreateRotationX((rand() % 256) / 255.0f);
//	matWorld = matScale * matRotation;
//
//	// OBB 기저벡터 변환( 회전 변환 이후 정규화 )
//	m_myBox.vCenter = m_vPosition;
//	Vector3 vMax, vMin, vHalf;
//	m_myBox.vAxis[0] = Vector3::Transform(Vector3(1.0f, 0.0f, 0.0f), matWorld);
//	m_myBox.vAxis[1] = Vector3::Transform(Vector3(0.0f, 1.0f, 0.0f), matWorld);
//	m_myBox.vAxis[2] = Vector3::Transform(Vector3(0.0f, 0.0f, 1.0f), matWorld);
//
//	m_myBox.vAxis[0].Normalize();
//	m_myBox.vAxis[1].Normalize();
//	m_myBox.vAxis[2].Normalize();
//	// g_matWorld Matrix = S * R * T
//	matWorld._41 = m_vPosition.x;
//	matWorld._42 = m_vPosition.y;
//	matWorld._43 = m_vPosition.z;
//
//	// OBB 기저벡터의 크기 계산
//	m_myBox.vMax = Vector3::Transform(m_myBox.vMax, matWorld);
//	m_myBox.vMin = Vector3::Transform(m_myBox.vMin, matWorld);
//	vHalf = vMax - m_myBox.vCenter;
//
//	m_myBox.fExtent[0] = m_myBox.vAxis[0].Dot(vHalf);
//	m_myBox.fExtent[1] = m_myBox.vAxis[1].Dot(vHalf);
//	m_myBox.fExtent[2] = m_myBox.vAxis[2].Dot(vHalf);
//
//	// AABB
//	Vector3 vVertex[8];
//	vVertex[0] = Vector3(m_myBox.vMin.x, m_myBox.vMax.y, m_myBox.vMin.z);
//	vVertex[1] = Vector3(m_myBox.vMax.x, m_myBox.vMax.y, m_myBox.vMin.z);
//	vVertex[2] = Vector3(m_myBox.vMax.x, m_myBox.vMin.y, m_myBox.vMin.z);
//	vVertex[3] = Vector3(m_myBox.vMin.x, m_myBox.vMin.y, m_myBox.vMin.z);
//
//	vVertex[4] = Vector3(m_myBox.vMin.x, m_myBox.vMax.y, m_myBox.vMax.z);
//	vVertex[5] = Vector3(m_myBox.vMax.x, m_myBox.vMax.y, m_myBox.vMax.z);
//	vVertex[6] = Vector3(m_myBox.vMax.x, m_myBox.vMin.y, m_myBox.vMax.z);
//	vVertex[7] = Vector3(m_myBox.vMin.x, m_myBox.vMin.y, m_myBox.vMax.z);
//
//	vMin = Vector3(100000.0f, 100000.0f, 100000.0f);
//	vMax = Vector3(-100000.0f, -100000.0f, -100000.0f);
//	for (int iVer = 0; iVer < 8; iVer++)
//	{
//		vMin = Vector3::Transform(vVertex[iVer], matWorld);
//		if (vVertex[iVer].x > vMax.x) vMax.x = vVertex[iVer].x;
//		if (vVertex[iVer].y > vMax.y) vMax.y = vVertex[iVer].y;
//		if (vVertex[iVer].z > vMax.z) vMax.z = vVertex[iVer].z;
//
//		if (vVertex[iVer].x < vMin.x) vMin.x = vVertex[iVer].x;
//		if (vVertex[iVer].y < vMin.y) vMin.y = vVertex[iVer].y;
//		if (vVertex[iVer].z < vMin.z) vMin.z = vVertex[iVer].z;
//	}
//
//	m_myBox.vMin = vMin;
//	m_myBox.vMax = vMax;
//	m_matWorld = matWorld;
//}
//
//myNode::myNode()
//{
//	m_dwDepth = 0;
//	m_isLeaf = FALSE;
//	m_myBox.vCenter = Vector3(0.0f, 0.0f, 0.0f);
//	m_myBox.vMax = Vector3(1.0f, 1.0f, 1.0f);
//	m_myBox.vMin = Vector3(-1.0f, -1.0f, -1.0f);
//	m_myBox.vAxis[0] = Vector3(1.0f, 0.0f, 0.0f);
//	m_myBox.vAxis[1] = Vector3(0.0f, 1.0f, 0.0f);
//	m_myBox.vAxis[2] = Vector3(0.0f, 0.0f, 1.0f);
//	m_myBox.fExtent[0] = 1;
//	m_myBox.fExtent[1] = 1;
//	m_myBox.fExtent[2] = 1;
//}
//myNode::~myNode()
//{
//	// 노드의 코너 정점 인덱스 및 위치
//	if (!m_CornerList.empty()) m_CornerList.clear();
//	if (!m_CornerIndex.empty()) m_CornerIndex.clear();	// 추가
//
//	for (int i = 0; i < m_ChildList.size(); i++)
//	{
//		SAFE_DEL(m_ChildList[i]);
//	}
//}
