#include "pch.h"
#include "Sample.h"

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
	m_pMiniMap->SetRect(Vector3(-0.75f, 0.75f, 0.0f), 0.25f);
	m_pMiniMap->Create(L"../../data/shader/BasisVS.txt", L"../../data/shader/BasisPS.txt", L"");

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
			//오브젝트 띄우기
			SampleIns temp;
			temp.matWorld._41 = vPick.x;
			temp.matWorld._42 = vPick.y;
			temp.matWorld._43 = vPick.z;
			temp.fTick = 0.0f;

			if (m_pTargetObject)
			{
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
	}

	m_pMiniMap->m_pTransform->SetMatrix(NULL,
		NULL,
		NULL);
	m_pMiniMap->Render(g_pImmediateContext);
	return true;
}

bool Sample::Release()
{
	g_FbxLoader.Release();
	return true;
}
