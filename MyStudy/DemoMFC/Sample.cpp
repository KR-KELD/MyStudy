#include "pch.h"
#include "Sample.h"

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
#pragma endregion

#pragma region MiniMap
	if (m_pMiniMap->Begin())
	{
		if (m_isCreate)
		{
			m_pMap->m_pTransform->SetMatrix(NULL,
				&m_pTopCamera->m_pTransform->m_matView,
				&m_pTopCamera->m_pTransform->m_matProj);
			g_pImmediateContext->PSSetShaderResources(1, 1, m_pMapTool->m_NormalTex.m_pSRV.GetAddressOf());
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
		g_pImmediateContext->PSSetShaderResources(1, 1, m_pMapTool->m_NormalTex.m_pSRV.GetAddressOf());
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
#pragma region HeightMapTex

		m_pHeightMini->Begin();
		m_pHeightMini->End();
		m_pHeightMini->m_pTransform->SetMatrix(NULL, NULL, NULL);
		m_pHeightMini->Update(g_pImmediateContext);
		m_pHeightMini->PreRender(g_pImmediateContext);
		m_pHeightMini->SettingPipeLine(g_pImmediateContext);

		g_pImmediateContext->PSSetShaderResources(0, 1,
			m_pMapTool->m_HeightTex.m_pSRV.GetAddressOf());

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
