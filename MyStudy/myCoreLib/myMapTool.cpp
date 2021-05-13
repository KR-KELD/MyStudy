#include "myMapTool.h"

bool myMapTool::Init()
{
	m_fRadius = 20.0f;
	m_fSpeed = 0.1f;
	m_eMakingMode = TERRAIN_UP;
	m_isPicking = false;
	m_isChangeData = false;

	m_HeightTex.SetDesc();
	m_HeightTex.Create();
	m_NormalTex.SetDesc();
	m_NormalTex.Create();
	SetNormalTex(g_pImmediateContext, m_NormalTex.m_pStaging.Get());
	g_pImmediateContext->CopyResource(m_NormalTex.m_pTexture.Get(), m_NormalTex.m_pStaging.Get());
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
				m_isPicking = true;
				m_SelectNodeList.push_back(pNode);
			}
		}
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
	if (m_isPicking)
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
		//Vector3 vLeftTop = Vector3(-99999.0f, 0.0f, 99999.0f);
		//Vector3 vRightBottom = Vector3(99999.0f, 0.0f, -99999.0f);
		//float fLeft = m_PickSphere.vCenter.x - m_fRadius;
		//float fBottom = m_PickSphere.vCenter.z - m_fRadius;
		//float fTop = m_PickSphere.vCenter.x + m_fRadius;
		//float fRight = m_PickSphere.vCenter.z + m_fRadius;
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
				//}
				//if (m_pMap->m_VertexList[iID].p.z < vLeftTop.z
				//	&& fTop < m_pMap->m_VertexList[iID].p.z)
				//{
				//	vLeftTop.z = m_pMap->m_VertexList[iID].p.z;
				//}

				//if (m_pMap->m_VertexList[iID].p.x < vRightBottom.x
				//	&& fBottom < m_pMap->m_VertexList[iID].p.x)
				//{
				//	vRightBottom.x = m_pMap->m_VertexList[iID].p.x;
				//}
				//if (m_pMap->m_VertexList[iID].p.z > vRightBottom.z
				//	&& fTop > m_pMap->m_VertexList[iID].p.z)
				//{
				//	vRightBottom.z = m_pMap->m_VertexList[iID].p.z;
				//}

#pragma endregion
				Vector3 vPos = m_pMap->m_VertexList[iID].p;
				Vector3 mPos = m_Mouse.m_vIntersectionPos;
				vPos.y = mPos.y = 0.0f;
				float fDist = (vPos - mPos).Length();
				if (fDist < m_fRadius)
				{
					m_isChangeData = true;
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
			m_pQuadTree->RepreshBindingObj(m_ControlNodeList[iNode]);
		}
		g_pImmediateContext->UpdateSubresource(
			m_pMap->m_pVertexBuffer.Get(), 0, NULL, &m_pMap->m_VertexList.at(0), 0, 0);
	}
	if (m_isChangeData && !m_isPicking)
	{
		SetHeightTex(g_pImmediateContext, m_HeightTex.m_pStaging.Get());
		g_pImmediateContext->CopyResource(m_HeightTex.m_pTexture.Get(), m_HeightTex.m_pStaging.Get());
		m_pMap->CalcPerVertexNormalsFastLookup();
		g_pImmediateContext->UpdateSubresource(
			m_pMap->m_pVertexBuffer.Get(), 0, NULL, &m_pMap->m_VertexList.at(0), 0, 0);

		SetHeightTex(g_pImmediateContext, m_NormalTex.m_pStaging.Get());
		g_pImmediateContext->CopyResource(m_NormalTex.m_pTexture.Get(), m_NormalTex.m_pStaging.Get());

		m_isChangeData = false;
	}
}

bool myMapTool::SetHeightTex(ID3D11DeviceContext * pImmediateContext, ID3D11Texture2D * pTexDest)
{
	D3D11_TEXTURE2D_DESC desc;
	pTexDest->GetDesc(&desc);

	D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
	if (SUCCEEDED(pImmediateContext->Map((ID3D11Resource*)pTexDest,
		0, D3D11_MAP_READ_WRITE, 0, &MappedFaceDest)))
	{
		BYTE* pDestBytes = (BYTE*)MappedFaceDest.pData;
		//텍스쳐 크기랑 맵크기 비율
		float fWidthRatio = m_pMap->m_iNumCellCols / (float)desc.Width;
		float fHeightRatio = m_pMap->m_iNumCellRows / (float)desc.Height;
		for (UINT y = 0; y < desc.Height; y++)
		{
			for (UINT x = 0; x < desc.Width; x++)
			{

				int iCol = x * fWidthRatio;
				int iRow = y * fHeightRatio;

				float h = m_pMap->GetHeightMap(iRow, iCol);

				*pDestBytes++ = h;
				*pDestBytes++ = h;
				*pDestBytes++ = h;
				*pDestBytes++ = 255;
			}
		}
		pImmediateContext->Unmap(pTexDest, 0);
	}
	return true;
}

bool myMapTool::SetHeightTex(ID3D11DeviceContext * pImmediateContext, ID3D11Texture2D * pTexDest, Vector3& vPick)
{
	D3D11_TEXTURE2D_DESC desc;
	pTexDest->GetDesc(&desc);
	float fL = vPick.x - m_fRadius;
	float fT = vPick.z + m_fRadius;
	float fR = vPick.x + m_fRadius;
	float fB = vPick.z - m_fRadius;

	D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
	if (SUCCEEDED(pImmediateContext->Map((ID3D11Resource*)pTexDest,
		0, D3D11_MAP_READ_WRITE, 0, &MappedFaceDest)))
	{
		BYTE* pDestBytes = (BYTE*)MappedFaceDest.pData;
		for (UINT y = 0; y < desc.Height; y++)
		{
			for (UINT x = 0; x < desc.Width; x++)
			{

			}
		}
		pImmediateContext->Unmap(pTexDest, 0);
	}
	return true;
}

bool myMapTool::SetNormalTex(ID3D11DeviceContext * pImmediateContext, ID3D11Texture2D * pTexDest)
{
	D3D11_TEXTURE2D_DESC desc;
	pTexDest->GetDesc(&desc);

	D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
	if (SUCCEEDED(pImmediateContext->Map((ID3D11Resource*)pTexDest,
		0, D3D11_MAP_READ_WRITE, 0, &MappedFaceDest)))
	{
		BYTE* pDestBytes = (BYTE*)MappedFaceDest.pData;
		for (UINT y = 0; y < desc.Height; y++)
		{
			for (UINT x = 0; x < desc.Width; x++)
			{

				*pDestBytes++ = 0;
				*pDestBytes++ = 0;
				*pDestBytes++ = 0;
				*pDestBytes++ = 0;
			}
		}
		pImmediateContext->Unmap(pTexDest, 0);
	}
	return true;
}

myMapTool::myMapTool(myMap* pMap, myQuadTree* pQuadTree)
{
	m_pMap = pMap;
	m_pQuadTree = pQuadTree;
}

myMapTool::~myMapTool()
{
}
