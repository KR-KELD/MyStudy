#include "pch.h"
#include "Sample.h"

bool Sample::SetHeightTex(ID3D11DeviceContext * pImmediateContext, ID3D11Texture2D * pTexDest)
{
	HRESULT hr;
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
		int a = 0;
		for (UINT y = 0; y < desc.Height; y++)
		{
			for (UINT x = 0; x < desc.Width; x++)
			{
				
				int iCol = x * fWidthRatio;
				int iRow = y * fHeightRatio;

				float h = m_pMap->GetHeightMap(iRow, iCol);
				h = (h + 100.0f) * 255.0f / 200.0f;
				*pDestBytes++ = h;
				*pDestBytes++ = h;
				*pDestBytes++ = h;
				*pDestBytes++ = 255;

				//*pDestBytes++ = a;
				//*pDestBytes++ = a;
				//*pDestBytes++ = a;
				//*pDestBytes++ = 255;
				//a++;
				//if (a > 200) a = 0;
			}
		}
		pImmediateContext->Unmap(pTexDest, 0);
	}
	return true;
}

bool Sample::SetTargetObject(string strName)
{
	//g_FbxLoader
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
	//껍데기

	HRESULT hr;
	//1 스테이징 텍스쳐 생성
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = 1024;
	desc.Height = 1024;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	desc.BindFlags = 0;
	desc.MiscFlags = 0;
	hr = g_pd3dDevice->CreateTexture2D(&desc, NULL, m_pStaging.GetAddressOf());
	// 새로 만든 텍스쳐(스테이징)빈거

	// 중요! 쉐이더 리소스 뷰가 되려면 반드시 D3D11_USAGE_DEFAULT이어야 한다.

	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;

	// 원본 이미지에 대한 밉맵 생성 되도록 설정한다.이때 
	//( 중요! 단, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET을 함께 사용하여야 한다.
	// 이렇게 하고 pImmediateContext->GenerateMips( pTextureRV )를 호출하면 밉맵이 생성된다.

	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	//2.쉐이더리소스랑 연결할 텍스쳐
	//새로 만든 쉐이더 리소스랑 연결할 텍스쳐
	hr = g_pd3dDevice->CreateTexture2D(&desc, NULL, m_pHeight.GetAddressOf());

	//쉐이더 리소스랑 연결
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));
	SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = 1;
	hr = g_pd3dDevice->CreateShaderResourceView(m_pHeight.Get(), &SRVDesc, m_pSRVHeight.GetAddressOf());
	   	  
	m_pTopCamera = new myCamera;
	g_CamMgr.CreateCameraObj(L"TopCamera", m_pTopCamera);

	m_pTopCamera->CreateViewMatrix({ 0,30.0f,-0.1f }, { 0,0,0 });
	float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	m_pTopCamera->CreateOrthographic(100, 100, 1.0f, 1000);

	
	myFbxObj* pFbxObj = g_FbxLoader.Load("../../data/object/Turret_Deploy1.fbx");
	pFbxObj->CuttingAnimScene(L"0", pFbxObj->m_AnimScene.iFirstFrame, pFbxObj->m_AnimScene.iLastFrame);
	pFbxObj->m_pModelObject->m_pAnim->ChangeAnim(L"0");
	m_DrawList.push_back(pFbxObj->m_pModelObject);
	pFbxObj = g_FbxLoader.Load("../../data/object/SM_Barrel.fbx");
	//pFbxObj->CuttingAnimScene(L"0", pFbxObj->m_AnimScene.iFirstFrame, pFbxObj->m_AnimScene.iLastFrame);
	m_DrawList.push_back(pFbxObj->m_pModelObject);

	m_isCreate = false;

	return true;
}

bool Sample::Frame()
{
	m_SelectNodeList.clear();
	if (m_isCreate)
	{
		if (g_Input.GetKey(VK_LBUTTON) == KEY_PUSH)
		{
			m_Mouse.ScreenToRay();
			for (int i = 0; i < m_QuadTree.m_LeafNodeList.size(); i++)
			{
				myNode* pNode = m_QuadTree.m_LeafNodeList[i];
				if (myCollision::IntersectRayToBox(m_Mouse.m_myRay, pNode->m_myBox/*,&m_Mouse.m_vIntersectionPos*/))
				{

					//임시
					//bool isSelected = false;
					//for (myNode* sNode : m_SelectNodeList)
					//{
					//	if (sNode == pNode)
					//	{
					//		isSelected = true;
					//	}
					//}
					//if (!isSelected)
					//	m_SelectNodeList.push_back(pNode);
					m_SelectNodeList.push_back(pNode);
				}
			}
		}

		Vector3 vPick;
		float fMaxDist = 99999.0f;
		bool bUpdate = false;

		for (myNode* pNode : m_SelectNodeList)
		{
			if (m_Mouse.PickingFace(pNode))
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
		if (bUpdate)
		{
			if (m_pTargetObject)
			{
				//오브젝트 띄우기
				SampleIns temp;
				temp.matWorld._41 = vPick.x;
				temp.matWorld._42 = vPick.y;
				temp.matWorld._43 = vPick.z;
				temp.fTick = 0.0f;

				if (m_pTargetObject->m_strName == L"../../data/object/SM_Barrel.fbx")
				{
					temp.matWorld._11 = 0.2f;
					temp.matWorld._22 = 0.2f;
					temp.matWorld._33 = 0.2f;
				}
				m_pTargetObject->m_InstanceList.push_back(temp);
			}

		}
	}
	return true;
}

bool Sample::Render()
{
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
		SetHeightTex(g_pImmediateContext, m_pStaging.Get());
		//사용할 텍스쳐를 스테이징으로 카피
		g_pImmediateContext->CopyResource(m_pHeight.Get(), m_pStaging.Get());
	}
	if (m_pMiniMap->Begin())
	{
		if (m_isCreate)
		{
			m_pMap->m_pTransform->SetMatrix(NULL,
				&m_pTopCamera->m_pTransform->m_matView,
				&m_pTopCamera->m_pTransform->m_matProj);
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



	if (m_isCreate)
	{
		//myDxState::g_RasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
		//myDxState::SetRasterizerState(g_pd3dDevice, g_pImmediateContext,
		//	myDxState::g_RasterizerDesc);

		m_pMap->m_pTransform->SetMatrix(NULL,
			&g_pMainCamTransform->m_matView,
			&g_pMainCamTransform->m_matProj);
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

		//myDxState::g_RasterizerDesc.FillMode = D3D11_FILL_SOLID;
		//myDxState::SetRasterizerState(g_pd3dDevice, g_pImmediateContext,
		//	myDxState::g_RasterizerDesc);

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

		m_pHeightMini->m_pTransform->SetMatrix(NULL,
			NULL,
			NULL);
		m_pHeightMini->Update(g_pImmediateContext);
		m_pHeightMini->PreRender(g_pImmediateContext);
		m_pHeightMini->SettingPipeLine(g_pImmediateContext);

		g_pImmediateContext->PSSetShaderResources(0, 1,
			m_pSRVHeight.GetAddressOf());

		m_pHeightMini->myGraphics::Draw(g_pImmediateContext);



		float fRadius = 20.0f;
		m_ControlNodeList.clear();
		MY_SPHERE pickSphere;
		pickSphere.vCenter = m_Mouse.m_vIntersectionPos;
		pickSphere.fRadius = 20.0f;
		for (int i = 0; i < m_QuadTree.m_LeafNodeList.size(); i++)
		{
			myNode* pNode = m_QuadTree.m_LeafNodeList[i];
			if (myCollision::IntersectSphereToSphere(pNode->m_mySphere, pickSphere))
			{
				m_ControlNodeList.push_back(pNode);
			}
		}
		for (auto node : m_ControlNodeList)
		{

			for (int v = 0; v < node->m_IndexList.size(); v++)
			{
				int iID = node->m_IndexList[v];
				float fDist = (m_pMap->m_VertexList[iID].p - m_Mouse.m_vIntersectionPos).Length();
				if (fDist < fRadius)
				{
					Vector3 v = m_pMap->m_VertexList[iID].p;
					m_pMap->m_VertexList[iID].p.y = v.y + 1.0f - sinf((fDist / fRadius));
				}
			}
			//m_Map.CalcPerVertexNormalsFastLookup();
		}
		g_pImmediateContext->UpdateSubresource(
			m_pMap->m_pVertexBuffer.Get(), 0, NULL, &m_pMap->m_VertexList.at(0), 0, 0);

	

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

	g_FbxLoader.Release();
	return true;
}
