#include "myMapTool.h"

bool myMapTool::Init()
{
	m_fRadius = 20.0f;
	m_fSpeed = 0.1f;
	m_eMakingMode = TERRAIN_UP;
	m_isPicking = false;
	return true;
}

bool myMapTool::Frame()
{
	if (g_Input.GetKey(VK_LBUTTON) == KEY_PUSH)
	{
		m_Mouse.ScreenToRay();
		for (int i = 0; i < m_pQuadTree->m_LeafNodeList.size(); i++)
		{
			myNode* pNode = m_pQuadTree->m_LeafNodeList[i];
			if (myCollision::IntersectRayToBox(m_Mouse.m_myRay, pNode->m_myBox/*,&m_Mouse.m_vIntersectionPos*/))
			{
				m_SelectNodeList.push_back(pNode);
			}
		}
		m_isPicking = true;
	}
	if (g_Input.GetKey(VK_LBUTTON) == KEY_UP)
	{
		m_isPicking = false;
	}
	Vector3 vPick;
	float fMaxDist = 99999.0f;
	bool bUpdate = false;
	for (int iNode = 0; iNode < m_SelectNodeList.size(); iNode++)
	{
		myNode* pNode = m_SelectNodeList[iNode];
		if (m_Mouse.PickingFace(pNode, m_pMap))
		{
			float fDist = (m_Mouse.m_vIntersectionPos - m_Mouse.m_myRay.vOrigin).Length();
			if (fDist < fMaxDist)
			{
				vPick = m_Mouse.m_vIntersectionPos;
				fMaxDist = fDist;
				bUpdate = true;
			}
		}
	}
	if (m_isPicking)
		EditTerrain();
	return true;
}

bool myMapTool::Render()
{

	return true;
}


bool myMapTool::Release()
{
	return true;
}

void myMapTool::SetMode(int iMode)
{
	m_eMakingMode = (ToolType)iMode;
}

void myMapTool::EditTerrain()
{
	m_ControlNodeList.clear();
	m_PickSphere.vCenter = m_Mouse.m_vIntersectionPos;
	m_PickSphere.fRadius = m_fRadius;
	for (int i = 0; i < m_pQuadTree->m_LeafNodeList.size(); i++)
	{
		myNode* pNode = m_pQuadTree->m_LeafNodeList[i];
		if (myCollision::IntersectSphereToSphere(pNode->m_mySphere, m_PickSphere))
		{
			m_ControlNodeList.push_back(pNode);
		}
	}
#pragma region test1

	//Vector3 vLeftTop = Vector3(-99999.0f, 0.0f, 99999.0f);
	//Vector3 vRightBottom = Vector3(99999.0f, 0.0f, -99999.0f);
	//Vector2 uvLT, uvRB;
	//float fLeft = m_PickSphere.vCenter.x - m_fRadius;
	//float fBottom = m_PickSphere.vCenter.z - m_fRadius;
	//float fTop = m_PickSphere.vCenter.x + m_fRadius;
	//float fRight = m_PickSphere.vCenter.z + m_fRadius;

#pragma endregion
	for (int iNode = 0; iNode < m_ControlNodeList.size(); iNode++)
	{
		for (int v = 0; v < m_ControlNodeList[iNode]->m_IndexList.size(); v++)
		{
			int iID = m_ControlNodeList[iNode]->m_IndexList[v];
#pragma region test2

			//if (m_pMap->m_VertexList[iID].p.x > vLeftTop.x
			//	&& fLeft > m_pMap->m_VertexList[iID].p.x)
			//{
			//	vLeftTop.x = m_pMap->m_VertexList[iID].p.x;
			//	uvLT.x = m_pMap->m_VertexList[iID].t.x;
			//}
			//if (m_pMap->m_VertexList[iID].p.z < vLeftTop.z
			//	&& fTop < m_pMap->m_VertexList[iID].p.z)
			//{
			//	vLeftTop.z = m_pMap->m_VertexList[iID].p.z;
			//	uvLT.y = m_pMap->m_VertexList[iID].t.y;
			//}

			//if (m_pMap->m_VertexList[iID].p.x < vRightBottom.x
			//	&& fBottom < m_pMap->m_VertexList[iID].p.x)
			//{
			//	vRightBottom.x = m_pMap->m_VertexList[iID].p.x;
			//	uvRB.x = m_pMap->m_VertexList[iID].t.x;
			//}
			//if (m_pMap->m_VertexList[iID].p.z > vRightBottom.z
			//	&& fTop > m_pMap->m_VertexList[iID].p.z)
			//{
			//	vRightBottom.z = m_pMap->m_VertexList[iID].p.z;
			//	uvRB.y = m_pMap->m_VertexList[iID].t.y;
			//}

#pragma endregion
			Vector3 vPos = m_pMap->m_VertexList[iID].p;
			Vector3 mPos = m_Mouse.m_vIntersectionPos;
			vPos.y = mPos.y = 0.0f;
			float fDist = (vPos - mPos).Length();
			if (fDist < m_fRadius)
			{
				Vector3 vp = m_pMap->m_VertexList[iID].p;
				switch (m_eMakingMode)
				{
				case TERRAIN_UP:
				{
					m_pMap->m_VertexList[iID].p.y = vp.y + m_fSpeed;
					if (m_pMap->m_VertexList[iID].p.y > 255.0f) m_pMap->m_VertexList[iID].p.y = 255.0f;
				}
				break;
				case TERRAIN_DOWN:
				{
					m_pMap->m_VertexList[iID].p.y = vp.y - m_fSpeed;
					if (m_pMap->m_VertexList[iID].p.y < 0.0f) m_pMap->m_VertexList[iID].p.y = 0.0f;
				}
				break;
				case TERRAIN_FLAT:
				{
					if (m_pMap->m_VertexList[iID].p.y < m_PickSphere.vCenter.y + m_fSpeed + 0.1f
						&& m_pMap->m_VertexList[iID].p.y > m_PickSphere.vCenter.y - m_fSpeed - 0.1f)
					{
						m_pMap->m_VertexList[iID].p.y = m_PickSphere.vCenter.y;
						break;
					}
					if (m_pMap->m_VertexList[iID].p.y < m_PickSphere.vCenter.y)
					{
						m_pMap->m_VertexList[iID].p.y = vp.y + m_fSpeed;
						if (m_pMap->m_VertexList[iID].p.y > 255.0f) m_pMap->m_VertexList[iID].p.y = 255.0f;
					}
					if (m_pMap->m_VertexList[iID].p.y > m_PickSphere.vCenter.y)
					{
						m_pMap->m_VertexList[iID].p.y = vp.y - m_fSpeed;
						if (m_pMap->m_VertexList[iID].p.y < 0.0f) m_pMap->m_VertexList[iID].p.y = 0.0f;
					}
				}
				break;
				default:
					break;
				}

			}
		}
		//m_Map.CalcPerVertexNormalsFastLookup();
		m_pQuadTree->RepreshBindingObj(m_ControlNodeList[iNode]);
	}
	g_pImmediateContext->UpdateSubresource(
		m_pMap->m_pVertexBuffer.Get(), 0, NULL, &m_pMap->m_VertexList.at(0), 0, 0);

}

myMapTool::myMapTool(myMap* pMap, myQuadTree* pQuadTree)
{
	m_pMap = pMap;
	m_pQuadTree = pQuadTree;
}

myMapTool::~myMapTool()
{
}
