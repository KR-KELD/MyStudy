#include "pch.h"
#include "Sample.h"

bool Sample::Init()
{

	m_pMap = new myHeightMap;
	g_ObjMgr.CreateObjComponent(L"Map", m_pMap, OBJECT_SUB);

	m_pMiniMap = new myMiniMap;
	g_ObjMgr.CreateObjComponent(L"MiniMap", m_pMiniMap, OBJECT_SUB);
	m_pMiniMap->SetInfo(Vector3(-0.75f, 0.75f, 0.0f), 0.25f);
	m_pMiniMap->Create(L"BasisVS.txt", L"BasisPS.txt", L"");

	m_pHeightMini = new myMiniMap;
	g_ObjMgr.CreateObjComponent(L"HeightMap", m_pHeightMini, OBJECT_SUB);
	m_pHeightMini->SetInfo(Vector3(0.75f, 0.75f, 0.0f), 0.25f);
	m_pHeightMini->Create(L"BasisVS.txt", L"BasisPS.txt", L"");

	m_pTopCamera = new myCamera;
	g_CamMgr.CreateCameraObj(L"TopCamera", m_pTopCamera);
	m_pTopCamera->CreateViewMatrix({ 0,500.0f,-0.1f }, { 0,0,0 });
	
	m_isCreate = false;
	return true;
}

bool Sample::Frame()
{
	
	if (m_isCreate)
	{
		g_CamMgr.m_pMainCamera->FrameFrustum();
		m_pMapTool->Frame();
	}
	return true;
}

bool Sample::Render()
{
#pragma region KeyControl
	if (g_Input.GetKey('0') == KEY_PUSH)
	{
		m_isDebugText = !m_isDebugText;
	}
	if (m_isCreate)
	{
		if (g_Input.GetKey(VK_F7) == KEY_PUSH)
		{
			m_QuadTree.m_pCullingCamera = g_CamList.GetGameObject(L"DebugCamera")->GetComponent<myCamera>();
		}
		if (g_Input.GetKey(VK_F8) == KEY_PUSH)
		{
			m_QuadTree.m_pCullingCamera = g_CamList.GetGameObject(L"TopCamera")->GetComponent<myCamera>();
		}
	}

	if (g_Input.GetKey('8') == KEY_PUSH)
	{
		myDxState::g_RasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
		//myDxState::SetRasterizerState(g_pd3dDevice, g_pImmediateContext,
		//	myDxState::g_RasterizerDesc);
	}
	if (g_Input.GetKey('9') == KEY_PUSH)
	{
		myDxState::g_RasterizerDesc.FillMode = D3D11_FILL_SOLID;
		//myDxState::SetRasterizerState(g_pd3dDevice, g_pImmediateContext,
		//	myDxState::g_RasterizerDesc);
	}
	//if (g_Input.GetKey('5') == KEY_PUSH)
	//{
	//	m_pMapTool->SetMode(300);
	//}
	//if (g_Input.GetKey('6') == KEY_PUSH)
	//{
	//	m_pMapTool->SetMode(301);
	//}
	//if (g_Input.GetKey('7') == KEY_PUSH)
	//{
	//	m_pMapTool->SetMode(302);
	//}
	//if (g_Input.GetKey('4') == KEY_PUSH)
	//{
	//	m_pMapTool->SetMode(304);
	//}

#pragma endregion
	myDxState::SetRasterizerState(g_pd3dDevice, g_pImmediateContext,
		myDxState::g_RasterizerDesc);
	g_CamMgr.SetMainCamera(L"TopCamera");
#pragma region MiniMap
	if (m_pMiniMap->Begin())
	{
		if (m_isCreate)
		{
			m_pMapTool->Render(g_pImmediateContext);
		}
		m_pMiniMap->End();
	}
	g_CamMgr.SetMainCamera(L"DebugCamera");
#pragma endregion
	if (m_isCreate)
	{
#pragma region Map&ObjectRender
		m_pMapTool->Render(g_pImmediateContext);

#pragma endregion

#pragma region SelectDraw

		////����Ʈ��� ��ο�
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

		g_pImmediateContext->RSSetState(myDxState::g_pRSSolid);

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
		m_pMiniMap->m_pTransform->SetMatrix(NULL,
			NULL,
			NULL);
		m_pMiniMap->Render(g_pImmediateContext);

		if (m_isDebugText)
		{
			m_pMapTool->DebugText();
		}
	}



	return true;
}

bool Sample::Release()
{
	SAFE_DEL(m_pMapTool);
	return true;
}
