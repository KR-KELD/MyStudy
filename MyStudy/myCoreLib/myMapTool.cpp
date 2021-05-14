#include "myMapTool.h"

bool myMapTool::Init()
{
	m_fRadius = 20.0f;
	m_fSpeed = 0.1f;
	m_eMakingMode = TOOL_TOPOLOGY;
	m_eTopologyType = TERRAIN_UP;
	m_eSplatType = SPLAT_TEX_01;
	m_isPicking = false;
	m_isChangeData = false;
	m_pSplatTex[0] = g_TextureMgr.Load(L"cncr21S.bmp")->m_pTextureSRV.Get();
	m_pSplatTex[1] = g_TextureMgr.Load(L"kgca08.bmp")->m_pTextureSRV.Get();
	m_pSplatTex[2] = g_TextureMgr.Load(L"metal.bmp")->m_pTextureSRV.Get();
	m_pSplatTex[3] = g_TextureMgr.Load(L"flagstone.bmp")->m_pTextureSRV.Get();
	m_HeightTex.SetDesc();
	m_HeightTex.Create();

	m_NormalTex.SetDesc();
	m_NormalTex.Create();
	ResetTex(g_pImmediateContext, m_NormalTex.m_pStaging.Get());
	g_pImmediateContext->CopyResource(m_NormalTex.m_pTexture.Get(), m_NormalTex.m_pStaging.Get());
	return true;
}

bool myMapTool::Frame()
{
#pragma region PickingMouse
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
#pragma endregion

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
	if (iMode >= 100 && iMode < 200)
	{
		m_eMakingMode = TOOL_TOPOLOGY;
		m_eTopologyType = (TopologyType)iMode;
	}
	if (iMode >= 200 && iMode < 300)
	{
		m_eMakingMode = TOOL_SPLAT;
		m_eSplatType = (SplatType)iMode;
	}
}

void myMapTool::EditTerrain()
{
	if (m_isPicking)
	{
		m_ControlNodeList.clear();
#pragma region Terrain_Topology
		if (m_eMakingMode == TOOL_TOPOLOGY)
		{
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
			for (int iNode = 0; iNode < m_ControlNodeList.size(); iNode++)
			{
				for (int v = 0; v < m_ControlNodeList[iNode]->m_IndexList.size(); v++)
				{
					int iID = m_ControlNodeList[iNode]->m_IndexList[v];
					Vector3 vPos = m_pMap->m_VertexList[iID].p;
					Vector3 mPos = m_Mouse.m_vIntersectionPos;
					vPos.y = mPos.y = 0.0f;
					float fDist = (vPos - mPos).Length();
					if (fDist < m_fRadius)
					{
						m_isChangeData = true;
						Vector3 vp = m_pMap->m_VertexList[iID].p;
						switch (m_eTopologyType)
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
#pragma endregion

	}
	else
	{
		if (m_isChangeData && m_eMakingMode == TOOL_TOPOLOGY)
		{
			m_pMap->CalcPerVertexNormalsFastLookup();
			g_pImmediateContext->UpdateSubresource(
				m_pMap->m_pVertexBuffer.Get(), 0, NULL, &m_pMap->m_VertexList.at(0), 0, 0);

			SetHeightTex(g_pImmediateContext, m_HeightTex.m_pStaging.Get());
			g_pImmediateContext->CopyResource(m_HeightTex.m_pTexture.Get(), m_HeightTex.m_pStaging.Get());
			m_isChangeData = false;
		}
		if (m_eMakingMode == TOOL_SPLAT)
		{
			SetNormalTex(g_pImmediateContext, m_NormalTex.m_pStaging.Get(), m_Mouse.m_vIntersectionPos);
			g_pImmediateContext->CopyResource(m_NormalTex.m_pTexture.Get(), m_NormalTex.m_pStaging.Get());
		}
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
			int iRow = y * fHeightRatio;
			for (UINT x = 0; x < desc.Width; x++)
			{
				int iCol = x * fWidthRatio;
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

bool myMapTool::SetNormalTex(ID3D11DeviceContext * pImmediateContext, ID3D11Texture2D * pTexDest, Vector3& vPick)
{
	D3D11_TEXTURE2D_DESC desc;
	pTexDest->GetDesc(&desc);
	Vector3 vCenter = Vector3(vPick.x, 0.0f, vPick.z);
	D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
	if (SUCCEEDED(pImmediateContext->Map((ID3D11Resource*)pTexDest,
		0, D3D11_MAP_READ_WRITE, 0, &MappedFaceDest)))
	{
		BYTE* pDestBytes = (BYTE*)MappedFaceDest.pData;
		float fWidthRatio = m_pMap->m_iNumCellCols / (float)desc.Width;
		float fHeightRatio = m_pMap->m_iNumCellRows / (float)desc.Height;

		for (UINT y = 0; y < desc.Height; y++)
		{
			int iRow = y * fHeightRatio;
			for (UINT x = 0; x < desc.Width; x++)
			{
				int iCol = x * fWidthRatio;
				Vector3 p = m_pMap->m_VertexList[iRow * m_pMap->m_iNumRows + iCol].p;
				p.y = 0.0f;
				float fDist = (p - vCenter).Length();
				if (fDist < m_fRadius)
				{
					float iRatio;
					if (fDist < m_fRadius / 2.0f)
					{
						iRatio = 255;
					}
					else
					{				//		0.5				127 = 255
										//10	//20		
						iRatio = (1.0f - fDist / m_fRadius * 2.0f) * 255;
					}
					switch (m_eSplatType)
					{
					case SPLAT_TEX_01:
					{
						*pDestBytes++ = (int)iRatio;
						pDestBytes += 3;
					}
						break;
					case SPLAT_TEX_02:
					{
						pDestBytes++;
						*pDestBytes++ = (int)iRatio;
						pDestBytes += 2;
					}
						break;
					case SPLAT_TEX_03:
					{
						pDestBytes += 2;
						*pDestBytes++ = (int)iRatio;
						pDestBytes++;
					}
						break;
					case SPLAT_TEX_04:
					{
						pDestBytes += 3;
						*pDestBytes++ = (int)iRatio;
					}
						break;
					default:
						break;
					}
					continue;
				}
				pDestBytes += 4;
			}
		}
		pImmediateContext->Unmap(pTexDest, 0);
	}
	return true;
}

bool myMapTool::ResetTex(ID3D11DeviceContext * pImmediateContext, ID3D11Texture2D * pTexDest)
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
