#include "myMapTool.h"
#include "myObjManager.h"

bool myMapTool::Init()
{
	m_vBrushRad.x = 20.0f;
	m_vBrushRad.y = 10.0f;
	m_fSpeed = 0.1f;
	m_eMakingMode = TOOL_TOPOLOGY;
	m_eTopologyType = TERRAIN_UP;
	m_eSplatType = SPLAT_TEX_01;
	m_eObjEditType = OBJECT_CREATE;
	m_isPicking = false;
	m_isUpdateData = false;
	m_isUpdatePick = false;
	m_pTargetIns = nullptr;
	m_pTargetObject = nullptr;
	m_isSelectObject = false;

	myFbxObj* pFbxObj = g_FbxLoader.Load("Turret_Deploy1.fbx");
	pFbxObj->CuttingAnimScene(L"0", pFbxObj->m_AnimScene.iFirstFrame, pFbxObj->m_AnimScene.iLastFrame);
	pFbxObj->m_pModelObject->ChangeAnim(L"0");
	m_BaseList.push_back(pFbxObj->m_pModelObject.get());

	pFbxObj = g_FbxLoader.Load("SM_Barrel.fbx");
	m_BaseList.push_back(pFbxObj->m_pModelObject.get());

	m_pSplatTex[0] = g_TextureMgr.Load(L"cncr21S.bmp")->m_pTextureSRV.Get();
	m_pSplatTex[1] = g_TextureMgr.Load(L"kgca08.bmp")->m_pTextureSRV.Get();
	m_pSplatTex[2] = g_TextureMgr.Load(L"metal.bmp")->m_pTextureSRV.Get();
	m_pSplatTex[3] = g_TextureMgr.Load(L"flagstone.bmp")->m_pTextureSRV.Get();

	return true;
}

bool myMapTool::Frame()
{
#pragma region PickingMouse
	m_SelectNodeList.clear();
	if (g_Input.GetKey(VK_LBUTTON) == KEY_HOLD)
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
		if (m_isPicking)
		{
			m_isUpdateData = true;
		}
		m_isPicking = false;
	}
	Vector3 vPick;
	float fMaxDist = 99999.0f;
	m_isUpdatePick = false;
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
				m_isUpdatePick = true;
			}
		}
	}

#pragma endregion
	if (m_eMakingMode == TOOL_OBJECT)
	{
		EditObject(vPick);
	}

	EditTerrain(vPick);


	return true;
}

bool myMapTool::Render(ID3D11DeviceContext*	pd3dContext)
{

	return true;
}


bool myMapTool::Release()
{
	return true;
}

void myMapTool::CreateTex(int iTexSize)
{
	m_iTexSizeX = m_iTexSizeY = iTexSize;
	m_sbSplat[0].vRadius = Vector2(0.0f, 0.0f);
	m_sbSplat[0].iIndex = 0;
	m_sbSplat[0].vPickPos = Vector2(0.0f, 0.0f);
	m_sbSplat[0].fTexHeight = iTexSize;
	m_sbSplat[0].fTexWidth = iTexSize;
	m_SplatCS.CreateStructuredBuffer(g_pd3dDevice, sizeof(SplatBuf), 1, &m_sbSplat[0]);
	m_SplatCS.CreateBufferSRV(g_pd3dDevice, m_SplatCS.m_pStBuff.Get());

	m_SplatCS.CreateTextureUAV(g_pd3dDevice,NULL, iTexSize, iTexSize);
	m_SplatCS.CreateComputeShader(g_pd3dDevice, L"SplatCS.txt", "CS");

	m_NormalTex.Create(iTexSize);
	m_HeightTex.Create(iTexSize);

	ID3D11ShaderResourceView* arraySRV[2] = {
	m_SplatCS.m_pSRVCopy.Get(),
	m_SplatCS.m_pStBuffSRV.Get(),
	};

	m_SplatCS.RunComputeShader(g_pImmediateContext, arraySRV, 2,
		m_iTexSizeX / 32, m_iTexSizeY / 32, 1);

	g_pImmediateContext->CopyResource(m_SplatCS.m_pTextureCopy.Get(), m_SplatCS.m_pTexture.Get());
}

void myMapTool::EditTopology(Vector3& vPick)
{
	Vector3 mPos = vPick;
	mPos.y = 0.0f;
	for (int iNode = 0; iNode < m_ControlNodeList.size(); iNode++)
	{
		for (int v = 0; v < m_ControlNodeList[iNode]->m_IndexList.size(); v++)
		{
			int iID = m_ControlNodeList[iNode]->m_IndexList[v];
			Vector3 vPos = m_pMap->m_VertexList[iID].p;
			vPos.y = 0.0f;
			
			float fDist = (vPos - mPos).Length();
			if (fDist < m_vBrushRad.x)
			{
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

void myMapTool::EditSplatting(Vector3& vPick)
{
	float fMapWidth = m_pMap->m_iNumCellCols * m_pMap->m_fCellDistance;
	float fWidthRatio = m_iTexSizeX / fMapWidth;
	Vector2 vCenter = Vector2((vPick.x + (fMapWidth / 2.0f)) * fWidthRatio,
		(-(vPick.z - (fMapWidth / 2.0f))) * fWidthRatio);

	m_sbSplat[0].vRadius = m_vBrushRad * fWidthRatio;
	m_sbSplat[0].iIndex = m_eSplatType - 200;

	m_sbSplat[0].vPickPos = vCenter;
	m_sbSplat[0].fTexHeight = m_iTexSizeX;
	m_sbSplat[0].fTexWidth = m_iTexSizeY;
	g_pImmediateContext->UpdateSubresource(m_SplatCS.m_pStBuff.Get(),
		0, NULL, &m_sbSplat, 0, 0);

	ID3D11ShaderResourceView* arraySRV[2] = {
	m_SplatCS.m_pSRVCopy.Get(),
	m_SplatCS.m_pStBuffSRV.Get(),
	};

	m_SplatCS.RunComputeShader(g_pImmediateContext, arraySRV,2,
		m_iTexSizeX / 32, m_iTexSizeY / 32, 1);

	g_pImmediateContext->CopyResource(m_SplatCS.m_pTextureCopy.Get(), m_SplatCS.m_pTexture.Get());


	//int iLastIndex = m_pMap->m_iNumCols * m_pMap->m_iNumRows - 1;
	//Vector2 LT = Vector2(m_pMap->m_VertexList[0].p.x, m_pMap->m_VertexList[0].p.z);
	//Vector2 RB = Vector2(m_pMap->m_VertexList[iLastIndex].p.x,
	//					m_pMap->m_VertexList[iLastIndex].p.z);
	//for (int iNode = 0; iNode < m_ControlNodeList.size(); iNode++)
	//{
	//	for (int v = 0; v < m_ControlNodeList[iNode]->m_IndexList.size(); v++)
	//	{
	//		int iID = m_ControlNodeList[iNode]->m_IndexList[v];
	//		Vector3 vPos = m_pMap->m_VertexList[iID].p;
	//		vPos.y = 0.0f;
	//		if (vPos.x > LT.x && vPos.x < vPick.x - m_fOutRad)
	//		{
	//			LT.x = vPos.x;
	//		}
	//		if (vPos.z < LT.y && vPos.z > vPick.z + m_fOutRad)
	//		{
	//			LT.y = vPos.z;
	//		}
	//		if (vPos.x < RB.x && vPos.x > vPick.x + m_fOutRad)
	//		{
	//			RB.x = vPos.x;
	//		}
	//		if (vPos.z > RB.y && vPos.z < vPick.z - m_fOutRad)
	//		{
	//			RB.y = vPos.z;
	//		}
	//	}
	//}
	//
	//SetNormalTex(g_pImmediateContext, m_NormalTex.m_pStaging.Get(), vPick, LT, RB);
	//g_pImmediateContext->CopyResource(m_NormalTex.m_pTexture.Get(), m_NormalTex.m_pStaging.Get());
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
	if (iMode >= 300 && iMode < 400)
	{
		m_eMakingMode = TOOL_OBJECT;
		m_eObjEditType = (ObjectEditType)iMode;
	}
}

void myMapTool::TerrainRender(ID3D11DeviceContext * pImmediateContext, myCamera * pTargetCamera)
{
	m_pMap->m_pTransform->SetMatrix(NULL,
		&pTargetCamera->m_pTransform->m_matView,
		&pTargetCamera->m_pTransform->m_matProj);
	//if (m_NormalTex.m_pSRV.Get())
	//{
	//	pImmediateContext->PSSetShaderResources(1, 1, m_NormalTex.m_pSRV.GetAddressOf());
	//}

	pImmediateContext->PSSetShaderResources(1, 1, m_SplatCS.m_pSRVCopy.GetAddressOf());
	pImmediateContext->PSSetShaderResources(2, 4, m_pSplatTex);
	m_pQuadTree->Render(pImmediateContext);
}

void myMapTool::EditTerrain(Vector3& vPick)
{
	if (m_isPicking)
	{
		m_ControlNodeList.clear();
#pragma region Terrain_Topology

		m_PickSphere.vCenter = m_Mouse.m_vIntersectionPos;
		m_PickSphere.fRadius = 30.0f;
		for (int i = 0; i < m_pQuadTree->m_LeafNodeList.size(); i++)
		{
			myNode* pNode = m_pQuadTree->m_LeafNodeList[i];
			if (myCollision::IntersectSphereToSphere(pNode->m_mySphere, m_PickSphere))
			{
				m_ControlNodeList.push_back(pNode);
			}
		}
		if (m_eMakingMode == TOOL_TOPOLOGY)
		{
			EditTopology(vPick);
		}
		if (m_eMakingMode == TOOL_SPLAT && m_isUpdatePick)
		{
			EditSplatting(vPick);
		}
#pragma endregion

	}
	else
	{
		if (m_isUpdateData)
		{
			if (m_eMakingMode == TOOL_TOPOLOGY)
			{
				m_pMap->CalcPerVertexNormalsFastLookup();
				g_pImmediateContext->UpdateSubresource(
					m_pMap->m_pVertexBuffer.Get(), 0, NULL, &m_pMap->m_VertexList.at(0), 0, 0);

				SetHeightTex(g_pImmediateContext, m_HeightTex.m_pStaging.Get());
				g_pImmediateContext->CopyResource(m_HeightTex.m_pTexture.Get(), m_HeightTex.m_pStaging.Get());
				m_isUpdateData = false;
			}
		}
	}
}

void myMapTool::EditObject(Vector3& vPick)
{
	if (m_isPicking)
	{
		if (m_eObjEditType == OBJECT_CREATE)
		{
			if (m_pTargetObject != nullptr && m_isUpdateData)
			{
				//SampleIns ins;
				//ins.fTick = 0.0f;
				//ins.vPos = vPick;
				//ins.vScale = Vector3::One;
				//ins.qRot = Quaternion::Identity;
				////if (m_pTargetObject->m_iObjectID == 1) ins.vScale = Vector3(0.2f, 0.2f, 0.2f);
				//ins.isActive = true;
				//ins.iID = m_pTargetObject->m_iObjectID;
				//float fScale = (ins.vScale.x + ins.vScale.y + ins.vScale.z) / 3.0f;
				//ins.SphereCollider = m_pTargetObject->m_SphereCollider;
				//ins.SphereCollider.fRadius *= fScale;
				//ins.SphereCollider.vCenter = ins.vPos;
				//ins.SphereCollider.vCenter.y += m_pTargetObject->m_SphereCollider.vCenter.y * ins.vScale.y;
				//m_pTargetObject->m_InstanceList.push_back(ins);

				//SampleIns ins;
				//ins.fTick = 0.0f;
				//ins.vPos = vPick;
				//ins.vScale = Vector3::One;
				//ins.qRot = Quaternion::Identity;
				////if (m_pTargetObject->m_iObjectID == 1) ins.vScale = Vector3(0.2f, 0.2f, 0.2f);
				//ins.isActive = true;
				//ins.iID = m_pTargetObject->m_iObjectID;
				//float fScale = (ins.vScale.x + ins.vScale.y + ins.vScale.z) / 3.0f;
				//ins.SphereCollider = m_pTargetObject->m_SphereCollider;
				//ins.SphereCollider.fRadius *= fScale;
				//ins.SphereCollider.vCenter = ins.vPos;
				//ins.SphereCollider.vCenter.y += m_pTargetObject->m_SphereCollider.vCenter.y * ins.vScale.y;
				//m_pTargetObject->m_InstanceList.push_back(ins);

				myModelObject* pObj = (myModelObject*)m_pTargetObject->CloneObject(m_pTargetObject.get());
				//pObj->m_pTransform = &pObj->m_TransForm;
				//myModelObject* pObj = (myModelObject*)m_pTargetObject->Clone(m_pTargetObject.get());
				pObj->m_pTransform->m_vPos = vPick;
				mySphereCollider* pCollider = pObj->GetComponent<mySphereCollider>();
				mySphereCollider* pBaseCollider = m_pTargetObject->GetComponent<mySphereCollider>();
				if (pCollider)
				{
					float fScale = (pObj->m_pTransform->m_vScale.x + 
									pObj->m_pTransform->m_vScale.y + 
									pObj->m_pTransform->m_vScale.z) / 3.0f;
					pCollider->m_Sphere.fRadius *= fScale;
					pCollider->m_Sphere.vCenter = vPick;
					pCollider->m_Sphere.vCenter.y +=
						pBaseCollider->m_Sphere.vCenter.y
						* pObj->m_pTransform->m_vScale.y;
				}
				m_DrawList.push_back(pObj);
				m_isUpdateData = false;
			}
		}
		else
		{
			if (!m_isSelectObject)
			{
				float fMaxDist = 99999.0f;
				for (int i = 0; i < m_DrawList.size(); i++)
				{
					for (int j = 0; j < m_DrawList[i]->m_InstanceList.size(); j++)
					{
						if (!m_DrawList[i]->m_InstanceList[j].isActive) continue;
						//if (myCollision::IntersectRayToBox(m_Mouse.m_myRay, m_DrawList[i]->m_InstanceList[j].myBoxCollider))
						//{
						//	float fDist = (m_Mouse.m_vIntersectionPos - m_Mouse.m_myRay.vOrigin).Length();
						//	if (fDist < fMaxDist)
						//	{
						//		m_pTargetIns = &m_DrawList[i]->m_InstanceList[j];
						//		fMaxDist = fDist;
						//		m_isSelectObject = true;
						//	}
						//}
						if (myCollision::IntersectRayToSphere(m_Mouse.m_myRay, m_DrawList[i]->m_InstanceList[j].SphereCollider))
						{
							float fDist = (m_Mouse.m_vIntersectionPos - m_Mouse.m_myRay.vOrigin).Length();
							if (fDist < fMaxDist)
							{
								m_Mouse.m_vPrevPos = m_Mouse.m_vIntersectionPos;
								m_pTargetIns = &m_DrawList[i]->m_InstanceList[j];
								fMaxDist = fDist;
								m_isSelectObject = true;
								m_vTemp = m_pTargetIns->vScale;
							}
						}
					}
				}
			}
			else
			{
				switch (m_eObjEditType)
				{
					break;
				case OBJECT_TRANS:
				{
					m_pTargetIns->vPos = vPick;
				}
				break;
				case OBJECT_SCALE:
				{
					Matrix mat;
					mat._41 = m_Mouse.m_vPrevPos.x; 
					mat._42 = m_Mouse.m_vPrevPos.y;
					mat._43 = m_Mouse.m_vPrevPos.z;
					mat = mat * g_pMainCamTransform->m_matView * g_pMainCamTransform->m_matProj;
					//Vector3 v1 = Vector3::Transform(m_Mouse.m_vPrevPos, mat);
					Vector2 v1;
					v1.x = ((mat._41 / mat._44) + 1.0f) * 0.5f * g_rtClient.right;
					v1.y = (1.0f - ((mat._42 / mat._44) + 1.0f) * 0.5f) * g_rtClient.bottom;
					//Vector3 v2 = Vector3::Transform(m_pTargetIns->vPos, mat);
					Vector2 v2;
					mat = Matrix::Identity;
					mat._41 = m_pTargetIns->vPos.x;
					mat._42 = m_pTargetIns->vPos.y;
					mat._43 = m_pTargetIns->vPos.z;
					mat = mat * g_pMainCamTransform->m_matView * g_pMainCamTransform->m_matProj;
					v2.x = ((mat._41 / mat._44) + 1.0f) * 0.5f * g_rtClient.right;
					v2.y = (1.0f - ((mat._42 / mat._44) + 1.0f) * 0.5f) * g_rtClient.bottom;
					float fOne = fabs(v1.x - v2.x);
					float fRange = fabs(g_Input.GetMouse().x - v2.x);
					//float fRange = (Vector2(g_Input.GetMouse().x, g_Input.GetMouse().y) - v2).Length();

					float fRatio = fRange / fOne;

					m_pTargetIns->vScale = m_vTemp * fRatio;
				}
				break;
				case OBJECT_ROTATE:
				{
					float fAngleX = (g_Input.GetMouse().x - g_CamMgr.m_pMainCamera->m_ptPrePosition.x);
					//float fAngleY = (g_Input.GetMouse().y - g_CamMgr.m_pMainCamera->m_ptPrePosition.y);

					m_pTargetIns->qRot.y -= XMConvertToRadians(fAngleX / 2.0f);
				}
				break;
				case OBJECT_DELETE:
				{
					m_pTargetIns->isActive = false;
				}
				break;
				default:
					break;
				}

			}
		}
	}
	else
	{
		if (m_isUpdateData && m_pTargetIns)
		{
			myModelObject* pObj = (myModelObject*)g_ObjMgr.GetGameObject(m_pTargetIns->iID);
			float fScale = (m_pTargetIns->vScale.x + m_pTargetIns->vScale.y + m_pTargetIns->vScale.z) / 3.0f;
			m_pTargetIns->SphereCollider = pObj->m_SphereCollider;
			m_pTargetIns->SphereCollider.fRadius *= fScale;
			m_pTargetIns->SphereCollider.vCenter = m_pTargetIns->vPos;
			m_pTargetIns->SphereCollider.vCenter.y += pObj->m_SphereCollider.vCenter.y * m_pTargetIns->vScale.y;
			m_pTargetIns = nullptr;
			m_isUpdateData = false;
			m_isSelectObject = false;
		}
	}
}

void myMapTool::ObjectRender(ID3D11DeviceContext * pImmediateContext, myCamera* pTargetCamera)
{
	for (int i = 0; i < m_DrawList.size(); i++)
	{
		m_DrawList[i]->m_pTransform->SetMatrix(NULL,
			&pTargetCamera->m_pTransform->m_matView,
			&pTargetCamera->m_pTransform->m_matProj);
		m_DrawList[i]->Render(pImmediateContext);

		//for (int j = 0; j < m_DrawList[i]->m_InstanceList.size(); j++)
		//{
		//	if (!m_DrawList[i]->m_InstanceList[j].isActive) continue;
		//	m_DrawList[i]->m_pTransform->m_vPos = m_DrawList[i]->m_InstanceList[j].vPos;
		//	m_DrawList[i]->m_pTransform->m_vScale = m_DrawList[i]->m_InstanceList[j].vScale;
		//	m_DrawList[i]->m_pTransform->m_qRot = m_DrawList[i]->m_InstanceList[j].qRot;
		//	m_DrawList[i]->m_pTransform->SetMatrix(NULL,
		//		&pTargetCamera->m_pTransform->m_matView,
		//		&pTargetCamera->m_pTransform->m_matProj);
		//	m_DrawList[i]->Render(pImmediateContext);
		//}
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

//bool myMapTool::SetNormalTex(ID3D11DeviceContext * pImmediateContext, ID3D11Texture2D * pTexDest, Vector3& vPick, Vector2 & vLT, Vector2 & vRB)
//{
//	D3D11_TEXTURE2D_DESC desc;
//	pTexDest->GetDesc(&desc);
//
//	D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
//	if (SUCCEEDED(pImmediateContext->Map((ID3D11Resource*)pTexDest,
//		0, D3D11_MAP_READ_WRITE, 0, &MappedFaceDest)))
//	{
//		BYTE* pDestBytes = (BYTE*)MappedFaceDest.pData;
//		float fMapWidth = m_pMap->m_iNumCellCols * m_pMap->m_fCellDistance;
//		float fWidthRatio = (float)desc.Width / fMapWidth;
//		UINT L = (vLT.x + (fMapWidth / 2.0f)) * fWidthRatio;
//		UINT R = (vRB.x + (fMapWidth / 2.0f)) * fWidthRatio;
//		UINT T = (-(vLT.y - (fMapWidth / 2.0f))) * fWidthRatio;
//		UINT B = (-(vRB.y - (fMapWidth / 2.0f))) * fWidthRatio;
//		Vector2 vCenter = Vector2((vPick.x + (fMapWidth / 2.0f)) * fWidthRatio,
//			(-(vPick.z - (fMapWidth / 2.0f))) * fWidthRatio);
//		float fRadius = m_vBrushRad.x * fWidthRatio;
//
//		for (UINT y = T; y < B; y++)
//		{
//			UINT rowStart = y * MappedFaceDest.RowPitch;
//			for (UINT x = L; x < R; x++)
//			{
//				UINT colStart = x * 4;
//				Vector2 p = Vector2(x, y);
//				float fDist = (p - vCenter).Length();
//				if (fDist < fRadius)
//				{
//					UINT iIndex = rowStart + colStart + m_eSplatType - 200;
//
//					float Lerf = 0.0f;
//					if (fDist < m_vBrushRad.y)
//					{
//						Lerf = 255.0f;
//					}
//					else
//					{
//						float fRatio = (fDist - m_vBrushRad.y) / (fRadius - m_vBrushRad.y);
//						Lerf = 255.0f * (1.0f - fRatio) + 0.0f * fRatio;
//					}
//					int iTemp = pDestBytes[iIndex];
//					if (iTemp > Lerf)
//					{
//						Lerf = Lerf / 20.0f;
//					}
//					pDestBytes[iIndex] = pDestBytes[iIndex] + (int)Lerf > 255 ?
//						255 : pDestBytes[iIndex] + (int)Lerf;
//				}
//			}
//		}
//		pImmediateContext->Unmap(pTexDest, 0);
//	}
//	return true;
//}

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

myMapTool::myMapTool(myHeightMap* pMap, myQuadTree* pQuadTree)
{
	m_pMap = pMap;
	m_pQuadTree = pQuadTree;
}

myMapTool::~myMapTool()
{
}
