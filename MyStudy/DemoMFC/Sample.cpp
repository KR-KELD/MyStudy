#include "pch.h"
#include "Sample.h"

bool Sample::SetHeightTex(ID3D11DeviceContext * pImmediateContext, ID3D11Texture2D * pTexDest)
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

bool Sample::SetHeightTex(ID3D11Texture2D * pTexDest, Vector3 & vLT, Vector3 & vRB)
{
	D3D11_TEXTURE2D_DESC desc;
	pTexDest->GetDesc(&desc);

	UINT iL = vLT.x * desc.Width;
	UINT iT = vLT.y * desc.Height;
	UINT iR = vRB.x * desc.Width;
	UINT iB = vRB.y * desc.Height;

	D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
	if (SUCCEEDED(g_pImmediateContext->Map((ID3D11Resource*)pTexDest,
		0, D3D11_MAP_READ_WRITE, 0, &MappedFaceDest)))
	{

		BYTE* pDestBytes = (BYTE*)MappedFaceDest.pData;
		pDestBytes += iT * MappedFaceDest.RowPitch;
		for (UINT y = iT; y < desc.Height; y++)
		{
			pDestBytes += iL * 4;
			for (UINT x = iL; x < desc.Width; x++)
			{
				if (y < iB)
				{
					if (x < iR)
					{
						*pDestBytes++ = 255;
						*pDestBytes++ = 255;
						*pDestBytes++ = 255;
						*pDestBytes++ = 255;
						continue;
					}
				}
				pDestBytes += 4;
			}
		}
		g_pImmediateContext->Unmap(pTexDest, 0);
	}
	return true;
}

bool Sample::SetNormalTex(ID3D11DeviceContext * pImmediateContext, ID3D11Texture2D * pTexDest)
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

bool Sample::Init()
{
	g_FbxLoader.Init();
	m_pMap = new myMap;
	m_pMapObj = myGameObject::CreateComponentObj(m_pMap);

	m_pMiniMap = new myMiniMap;
	m_pMiniMapObj = myGameObject::CreateComponentObj(m_pMiniMap);
	m_pMiniMap->SetInfo(Vector3(-0.75f, 0.75f, 0.0f), 0.25f);
	m_pMiniMap->Create(L"../../data/shader/BasisVS.txt", L"../../data/shader/BasisPS.txt", L"");

	m_pHeightMini = new myMiniMap;
	m_pHeightMiniObj = myGameObject::CreateComponentObj(m_pHeightMini);
	m_pHeightMini->SetInfo(Vector3(0.75f, 0.75f, 0.0f), 0.25f);
	m_pHeightMini->Create(L"../../data/shader/BasisVS.txt", L"../../data/shader/BasisPS.txt", L"");

	m_HeightTex.SetDesc();
	m_HeightTex.Create();

	m_NormalTex.SetDesc();
	m_NormalTex.Create();
	SetNormalTex(g_pImmediateContext, m_NormalTex.m_pStaging.Get());
	g_pImmediateContext->CopyResource(m_NormalTex.m_pTexture.Get(), m_NormalTex.m_pStaging.Get());

	m_pTopCamera = new myCamera;
	g_CamMgr.CreateCameraObj(L"TopCamera", m_pTopCamera);

	m_pTopCamera->CreateViewMatrix({ 0,500.0f,-0.1f }, { 0,0,0 });
	
	myFbxObj* pFbxObj = g_FbxLoader.Load("../../data/object/Turret_Deploy1.fbx");
	pFbxObj->CuttingAnimScene(L"0", pFbxObj->m_AnimScene.iFirstFrame, pFbxObj->m_AnimScene.iLastFrame);
	pFbxObj->m_pModelObject->m_pAnim->ChangeAnim(L"0");
	m_DrawList.push_back(pFbxObj->m_pModelObject);
	pFbxObj = g_FbxLoader.Load("../../data/object/SM_Barrel.fbx");
	//pFbxObj->CuttingAnimScene(L"0", pFbxObj->m_AnimScene.iFirstFrame, pFbxObj->m_AnimScene.iLastFrame);
	m_DrawList.push_back(pFbxObj->m_pModelObject);

	m_isCreate = false;
	m_isPicking = false;
	return true;
}

bool Sample::Frame()
{
//#pragma region Picking
//	m_SelectNodeList.clear();
//	if (m_isCreate)
//	{
//		if (g_Input.GetKey(VK_LBUTTON) == KEY_PUSH)
//		{
//			m_Mouse.ScreenToRay();
//			for (int i = 0; i < m_QuadTree.m_LeafNodeList.size(); i++)
//			{
//				myNode* pNode = m_QuadTree.m_LeafNodeList[i];
//				if (myCollision::IntersectRayToBox(m_Mouse.m_myRay, pNode->m_myBox/*,&m_Mouse.m_vIntersectionPos*/))
//				{
//					m_SelectNodeList.push_back(pNode);
//					m_isPicking = true;
//				}
//			}
//		}
//		if (g_Input.GetKey(VK_LBUTTON) == KEY_UP)
//		{
//			m_isPicking = false;
//		}
//
//		Vector3 vPick;
//		float fMaxDist = 99999.0f;
//		bool bUpdate = false;
//
//		for (int iNode = 0; iNode < m_SelectNodeList.size(); iNode++)
//		{
//			myNode* pNode = m_SelectNodeList[iNode];
//			if (m_Mouse.PickingFace(pNode, m_pMap))
//			{
//				float fDist = (m_Mouse.m_vIntersectionPos - m_Mouse.m_myRay.vOrigin).Length();
//				if (fDist < fMaxDist)
//				{
//					vPick = m_Mouse.m_vIntersectionPos;
//					fMaxDist = fDist;
//					bUpdate = true;
//				}
//			}
//		}
//		if (bUpdate)
//		{
//			if (m_pTargetObject)
//			{
//				//오브젝트 띄우기
//				SampleIns temp;
//				temp.matWorld._41 = vPick.x;
//				temp.matWorld._42 = vPick.y;
//				temp.matWorld._43 = vPick.z;
//				temp.fTick = 0.0f;
//
//				if (m_pTargetObject->m_strName == L"../../data/object/SM_Barrel.fbx")
//				{
//					temp.matWorld._11 = 0.2f;
//					temp.matWorld._22 = 0.2f;
//					temp.matWorld._33 = 0.2f;
//				}
//				m_pTargetObject->m_InstanceList.push_back(temp);
//			}
//
//		}
//	}
//#pragma endregion
	if (m_pMapTool) m_pMapTool->Frame();
	return true;
}

bool Sample::Render()
{
#pragma region KeyControl
	if (g_Input.GetKey('8') == KEY_PUSH)
	{
		myDxState::g_RasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
		myDxState::SetRasterizerState(g_pd3dDevice, g_pImmediateContext,
			myDxState::g_RasterizerDesc);
	}
	if (g_Input.GetKey('9') == KEY_PUSH)
	{
		myDxState::g_RasterizerDesc.FillMode = D3D11_FILL_SOLID;
		myDxState::SetRasterizerState(g_pd3dDevice, g_pImmediateContext,
			myDxState::g_RasterizerDesc);
	}
	if (g_Input.GetKey('0') == KEY_PUSH)
	{
		//스테이징 텍스쳐를 바꾸는 함수
		SetHeightTex(g_pImmediateContext, m_HeightTex.m_pStaging.Get());
		//사용할 텍스쳐를 스테이징으로 카피
		g_pImmediateContext->CopyResource(m_HeightTex.m_pTexture.Get(), m_HeightTex.m_pStaging.Get());
	}
#pragma endregion

#pragma region MiniMap
	if (m_pMiniMap->Begin())
	{
		if (m_isCreate)
		{
			m_pMap->m_pTransform->SetMatrix(NULL,
				&m_pTopCamera->m_pTransform->m_matView,
				&m_pTopCamera->m_pTransform->m_matProj);
			g_pImmediateContext->PSSetShaderResources(1, 1, m_NormalTex.m_pSRV.GetAddressOf());
			m_QuadTree.Render(g_pImmediateContext);
			for (int i = 0; i < m_DrawList.size(); i++)
			{
				for (int j = 0; j < m_DrawList[i]->m_InstanceList.size(); j++)
				{
					m_DrawList[i]->m_pTransform->SetMatrix(&m_DrawList[i]->m_InstanceList[j].matWorld,
						&m_pTopCamera->m_pTransform->m_matView,
						&m_pTopCamera->m_pTransform->m_matProj);
					m_DrawList[i]->Render();
				}
			}
		}
		m_pMiniMap->End();
	}
#pragma endregion
	if (m_isCreate)
	{
#pragma region Map&ObjectRender
		m_pMap->m_pTransform->SetMatrix(NULL,
			&g_pMainCamTransform->m_matView,
			&g_pMainCamTransform->m_matProj);
		g_pImmediateContext->PSSetShaderResources(1, 1, m_NormalTex.m_pSRV.GetAddressOf());
		m_QuadTree.Render(g_pImmediateContext);

		for (int i = 0; i < m_DrawList.size(); i++)
		{
			for (int j = 0; j < m_DrawList[i]->m_InstanceList.size(); j++)
			{
				m_DrawList[i]->m_pAnim->m_fTick = m_DrawList[i]->m_InstanceList[j].fTick;
				m_DrawList[i]->Frame();
				m_DrawList[i]->m_InstanceList[j].fTick = m_DrawList[i]->m_pAnim->m_fTick;
				m_DrawList[i]->m_pTransform->SetMatrix(&m_DrawList[i]->m_InstanceList[j].matWorld,
					&g_pMainCamTransform->m_matView,
					&g_pMainCamTransform->m_matProj);
				m_DrawList[i]->Render();
			}
		}

#pragma endregion

#pragma region SelectDraw

		////셀렉트노드 드로우
		//m_pMap->Update(g_pImmediateContext);
		//m_pMap->PreRender(g_pImmediateContext);
		//m_pMap->SettingPipeLine(g_pImmediateContext);
		//g_pImmediateContext->IASetIndexBuffer(NULL, DXGI_FORMAT_R32_UINT, 0);
		//for (myNode* pNode : m_SelectNodeList)
		//{
		//	g_pImmediateContext->IASetIndexBuffer(pNode->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		//	g_pImmediateContext->DrawIndexed(pNode->m_IndexList.size(), 0, 0);

		//}
#pragma endregion

//#pragma region TerrainTool
//
//		if (m_isPicking)
//		{
//			bool isControl = false;
//
//			float fRadius = 50.0f;
//			m_ControlNodeList.clear();
//			MY_SPHERE pickSphere;
//			pickSphere.vCenter = m_Mouse.m_vIntersectionPos;
//			pickSphere.fRadius = 20.0f;
//			for (int i = 0; i < m_QuadTree.m_LeafNodeList.size(); i++)
//			{
//				myNode* pNode = m_QuadTree.m_LeafNodeList[i];
//				if (myCollision::IntersectSphereToSphere(pNode->m_mySphere, pickSphere))
//				{
//					m_ControlNodeList.push_back(pNode);
//				}
//			}
//			Vector3 vLeftTop = Vector3(-99999.0f, 0.0f, 99999.0f);
//			Vector3 vRightBottom = Vector3(99999.0f, 0.0f, -99999.0f);
//			Vector2 uvLT, uvRB;
//			float fLeft = pickSphere.vCenter.x - pickSphere.fRadius;
//			float fBottom = pickSphere.vCenter.z - pickSphere.fRadius;
//			float fTop = pickSphere.vCenter.x + pickSphere.fRadius;
//			float fRight = pickSphere.vCenter.z + pickSphere.fRadius;
//
//
//			for (int iNode = 0; iNode < m_ControlNodeList.size(); iNode++)
//			{
//				for (int v = 0; v < m_ControlNodeList[iNode]->m_IndexList.size(); v++)
//				{
//					int iID = m_ControlNodeList[iNode]->m_IndexList[v];
//					if (m_pMap->m_VertexList[iID].p.x > vLeftTop.x
//						&& fLeft > m_pMap->m_VertexList[iID].p.x)
//					{
//						vLeftTop.x = m_pMap->m_VertexList[iID].p.x;
//						uvLT.x = m_pMap->m_VertexList[iID].t.x;
//					}
//					if (m_pMap->m_VertexList[iID].p.z < vLeftTop.z
//						&& fTop < m_pMap->m_VertexList[iID].p.z)
//					{
//						vLeftTop.z = m_pMap->m_VertexList[iID].p.z;
//						uvLT.y = m_pMap->m_VertexList[iID].t.y;
//					}
//
//					if (m_pMap->m_VertexList[iID].p.x < vRightBottom.x
//						&& fBottom < m_pMap->m_VertexList[iID].p.x)
//					{
//						vRightBottom.x = m_pMap->m_VertexList[iID].p.x;
//						uvRB.x = m_pMap->m_VertexList[iID].t.x;
//					}
//					if (m_pMap->m_VertexList[iID].p.z > vRightBottom.z
//						&& fTop > m_pMap->m_VertexList[iID].p.z)
//					{
//						vRightBottom.z = m_pMap->m_VertexList[iID].p.z;
//						uvRB.y = m_pMap->m_VertexList[iID].t.y;
//					}
//
//
//					float fDist = (m_pMap->m_VertexList[iID].p - m_Mouse.m_vIntersectionPos).Length();
//					if (fDist < fRadius)
//					{
//						Vector3 vp = m_pMap->m_VertexList[iID].p;
//						m_pMap->m_VertexList[iID].p.y = vp.y + 0.1f;
//						if (m_pMap->m_VertexList[iID].p.y > 255.0f) m_pMap->m_VertexList[iID].p.y = 255.0f;
//						isControl = true;
//					}
//
//
//				}
//				//m_Map.CalcPerVertexNormalsFastLookup();
//				m_QuadTree.RepreshBindingObj(m_ControlNodeList[iNode]);
//			}
//			g_pImmediateContext->UpdateSubresource(
//				m_pMap->m_pVertexBuffer.Get(), 0, NULL, &m_pMap->m_VertexList.at(0), 0, 0);
//
//			if (isControl)
//			{
//				SetHeightTex(m_HeightTex.m_pStaging.Get(), uvLT, uvRB);
//				g_pImmediateContext->CopyResource(m_HeightTex.m_pTexture.Get(), m_HeightTex.m_pStaging.Get());
//			}
//		}
//
//
//
//#pragma endregion

#pragma region HeightMapTex

		m_pHeightMini->Begin();
		m_pHeightMini->End();
		m_pHeightMini->m_pTransform->SetMatrix(NULL, NULL, NULL);
		m_pHeightMini->Update(g_pImmediateContext);
		m_pHeightMini->PreRender(g_pImmediateContext);
		m_pHeightMini->SettingPipeLine(g_pImmediateContext);

		g_pImmediateContext->PSSetShaderResources(0, 1,
			m_HeightTex.m_pSRV.GetAddressOf());

		m_pHeightMini->myGraphics::Draw(g_pImmediateContext);
#pragma endregion

	}

	m_pMiniMap->m_pTransform->SetMatrix(NULL,
		NULL,
		NULL);
	m_pMiniMap->Render(g_pImmediateContext);

	return true;
}

bool Sample::Release()
{
	m_pMiniMapObj->Release();
	m_pMapObj->Release();
	m_pHeightMiniObj->Release();
	SAFE_DEL(m_pHeightMiniObj);
	SAFE_DEL(m_pMiniMapObj);
	SAFE_DEL(m_pMapObj);
	SAFE_DEL(m_pMapTool);

	g_FbxLoader.Release();
	return true;
}
