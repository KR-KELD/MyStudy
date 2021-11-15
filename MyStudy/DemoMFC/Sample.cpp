#include "pch.h"
#include "Sample.h"

bool Sample::Init()
{
	m_eMinimapType = eTypeMinimap::RENDER_NONE;
	m_pMap = new myHeightMap;
	g_ObjMgr.CreateObjComponent(L"Map", m_pMap, OBJECT_SUB);

	m_pMiniMap = new myMiniMap;
	g_ObjMgr.CreateObjComponent(L"MiniMap", m_pMiniMap, OBJECT_SUB);
	m_pMiniMap->SetInfo(Vector3(-0.75f, 0.75f, 0.0f), 0.25f);
	m_pMiniMap->Create(L"BasisVS.txt", L"BasisPS.txt", L"");



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
	if (g_Input.GetKey('0') == KEY_PUSH)
	{
		m_isDebugText = !m_isDebugText;
	}
	if (m_isCreate)
	{
		if (g_Input.GetKey(VK_F1) == KEY_PUSH)
		{
			m_eMinimapType = eTypeMinimap::RENDER_NONE;
		}
		if (g_Input.GetKey(VK_F2) == KEY_PUSH)
		{
			m_eMinimapType = eTypeMinimap::RENDER_NORMAL;
		}
		if (g_Input.GetKey(VK_F3) == KEY_PUSH)
		{
			m_eMinimapType = eTypeMinimap::RENDER_DEPTH;
		}
		if (g_Input.GetKey(VK_F4) == KEY_PUSH)
		{
			m_eMinimapType = eTypeMinimap::RENDER_HEIGHT;
		}
		if (g_Input.GetKey(VK_F5) == KEY_PUSH)
		{
			m_QuadTree.m_pCullingCamera = g_CamList.GetGameObject(L"DebugCamera")->GetComponent<myCamera>();
		}
		if (g_Input.GetKey(VK_F6) == KEY_PUSH)
		{
			m_QuadTree.m_pCullingCamera = g_CamList.GetGameObject(L"TopCamera")->GetComponent<myCamera>();
		}
		if (g_Input.GetKey(VK_F7) == KEY_PUSH)
		{
			myDxState::g_RasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
			//myDxState::SetRasterizerState(g_pd3dDevice, g_pImmediateContext,
			//	myDxState::g_RasterizerDesc);
			int a = 0;
		}
		if (g_Input.GetKey(VK_F8) == KEY_PUSH)
		{
			myDxState::g_RasterizerDesc.FillMode = D3D11_FILL_SOLID;
			//myDxState::SetRasterizerState(g_pd3dDevice, g_pImmediateContext,
			//	myDxState::g_RasterizerDesc);
			int a = 0;
		}
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

	myDxState::SetRasterizerState(g_pd3dDevice, g_pImmediateContext,
		myDxState::g_RasterizerDesc);
	g_CamMgr.SetMainCamera(L"TopCamera");
	if (m_pMiniMap->Begin())
	{
	
		if (m_isCreate)
		{
			if (m_eMinimapType == eTypeMinimap::RENDER_NORMAL)
			{
				m_pMapTool->Render(g_pImmediateContext);
			}
		}
		m_pMiniMap->End();
	}
	g_CamMgr.SetMainCamera(L"DebugCamera");

	if (m_isCreate)
	{
		m_pMapTool->Render(g_pImmediateContext);
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

		g_pImmediateContext->RSSetState(myDxState::g_pRSSolid);



		if (m_eMinimapType != eTypeMinimap::RENDER_NONE)
		{
			m_pMiniMap->m_pTransform->SetMatrix(NULL,
				NULL,
				NULL);
			m_pMiniMap->Update(g_pImmediateContext);
			m_pMiniMap->PreRender(g_pImmediateContext);
			m_pMiniMap->SettingPipeLine(g_pImmediateContext);

			switch (m_eMinimapType)
			{

				case eTypeMinimap::RENDER_NORMAL:
				{
					g_pImmediateContext->PSSetShaderResources(0, 1,
						m_pMiniMap->m_DxRT.m_pSRV.GetAddressOf());
				}
				break;
				case eTypeMinimap::RENDER_DEPTH:
				{
					g_pImmediateContext->PSSetShaderResources(0, 1,
						m_QuadTree.m_pDepthMap->m_pRT->m_pSRV.GetAddressOf());
				}
				break;
				case eTypeMinimap::RENDER_HEIGHT:
				{
					g_pImmediateContext->PSSetShaderResources(0, 1,
						m_pMapTool->m_HeightTex.m_pSRV.GetAddressOf());
				}
				break;

				default:
					break;
			}
			m_pMiniMap->myGraphics::Draw(g_pImmediateContext);
		}

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
