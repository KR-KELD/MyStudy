#include "Sample.h"
GAMERUN;

bool Sample::Init()
{
	m_pDepthMap = new myDepthMap;
	m_pDepthMap->Init();

	m_pShadowMinimap = new myMiniMap;
	g_ObjMgr.CreateObjComponent(L"ShadowMap", m_pShadowMinimap, OBJECT_SUB);
	m_pShadowMinimap->SetInfo(Vector3(-0.75f, 0.75f, 0.0f), 0.25f);
	m_pShadowMinimap->Create(L"BasisVS.txt", L"BasisPS.txt", L"");

	m_pLight = new myCamera;
	myGameObject* obj = g_CamMgr.CreateCameraObj(L"LightCamera", m_pLight);
	m_pLight->CreateViewMatrix({ 200,200, 0 }, { 0,0,0 });
	m_pLight->CreateProjMatrix(1.0f, 1000.0f, PI4D, 1.0f);

	m_pFbxObj = g_FbxLoader.Load("../../data/object/man.fbx");
	m_pModelObj = m_pFbxObj->m_pModelObject;
	m_pModelObj->m_pNormalTex = g_TextureMgr.Load(L"../../data/object/base_normal_map.bmp");
	m_pFbxObj->CuttingAnimScene(L"0", 0, 60);
	m_pModelObj->ChangeAnim(L"0");
	m_pMap = new myMap;
	g_ObjMgr.CreateObjComponent(L"Map", m_pMap, OBJECT_SUB);

	myMapDesc desc;
	desc.iNumCols = 65;
	desc.iNumRows = 65;
	desc.fCellDistance = 7;
	desc.fScaleHeight = 10.0f;
	desc.szTexFile = L"main.png";
	desc.szVS = L"MapTestVS.txt";
	desc.szPS = L"MapTestPS.txt";
	m_pMap->CreateMap(desc);
	m_pMap->SetMapCBData(8, 8, 4, 1);

	//pApp->m_Sample.m_QuadTree.CreateQuadTree(pApp->m_Sample.m_pMap);
	//pApp->m_Sample.m_isCreate = true;
	//pApp->m_Sample.m_pMap->SetMapCBData(iNumCell, iNumTile, m_iCellSize, iSpaceDivision);
	//g_CamMgr.SetMainCamera(L"LightCamera");
	return true;
}

bool Sample::Frame()
{
	m_pModelObj->Frame();
	Matrix Trans = m_pLight->m_pTransform->m_matWorld * Matrix::CreateRotationY(PI4D * g_fSecondPerFrame);
	m_pLight->m_pTransform->m_vPos = Vector3::Transform(m_pLight->m_pTransform->m_vPos, Trans);

	m_pLight->Frame();
	m_pDepthMap->m_cbData.g_matShadow = m_pLight->m_pTransform->m_matView *
		m_pLight->m_pTransform->m_matProj * m_pDepthMap->m_matShadowTex;

	return true;
}

bool Sample::PreFrame()
{
	m_pModelObj->PreFrame();
	return true;
}

bool Sample::Render()
{
	//±×¸²ÀÚ¸Ê ·£´õÅ¸°Ù ÀÛ¼º
	g_pImmediateContext->RSSetState(myDxState::g_pRSSlopeScaledDepthBias);
	if (m_pDepthMap->m_pRT->Begin())
	{
		m_pModelObj->m_pTransform->SetMatrix(NULL,
			&m_pLight->m_pTransform->m_matView,
			&m_pLight->m_pTransform->m_matProj);

		m_pModelObj->m_pGraphics->m_isShadowRender = true;
		m_pModelObj->Render(g_pImmediateContext);
		m_pModelObj->m_pGraphics->m_isShadowRender = false;

		m_pMap->m_pTransform->SetMatrix(NULL,
			&m_pLight->m_pTransform->m_matView,
			&m_pLight->m_pTransform->m_matProj);

		m_pMap->m_isShadowRender = true;
		m_pMap->Render(g_pImmediateContext);
		m_pMap->m_isShadowRender = false;

		m_pDepthMap->m_pRT->End();
	}
	m_pDepthMap->m_cbData.g_matShadow = m_pDepthMap->m_cbData.g_matShadow.Transpose();
	g_pImmediateContext->UpdateSubresource(m_pDepthMap->m_pCBDepthMap.Get(), 0, NULL, &m_pDepthMap->m_cbData, 0, 0);
	g_pImmediateContext->VSSetConstantBuffers(2, 1, m_pDepthMap->m_pCBDepthMap.GetAddressOf());

	g_pImmediateContext->RSSetState(myDxState::g_pRSSolid);
	g_pImmediateContext->PSSetSamplers(1, 1, &myDxState::g_pSSClampLinear);

	m_pModelObj->m_pTransform->SetMatrix(NULL,
		&g_pMainCamTransform->m_matView,
		&g_pMainCamTransform->m_matProj);

	m_pModelObj->m_pGraphics->m_cbData.vColor[0] = m_pLight->m_pTransform->m_vLook.x;
	m_pModelObj->m_pGraphics->m_cbData.vColor[1] = m_pLight->m_pTransform->m_vLook.y;
	m_pModelObj->m_pGraphics->m_cbData.vColor[2] = m_pLight->m_pTransform->m_vLook.z;
	m_pModelObj->PreRender(g_pImmediateContext);
	g_pImmediateContext->PSSetShaderResources(2, 1,
		m_pDepthMap->m_pRT->m_pSRV.GetAddressOf());
	m_pModelObj->PostRender(g_pImmediateContext);

	m_pMap->m_pTransform->SetMatrix(NULL,
		&g_pMainCamTransform->m_matView,
		&g_pMainCamTransform->m_matProj);

	m_pMap->m_cbData.vColor[0] = m_pLight->m_pTransform->m_vLook.x;
	m_pMap->m_cbData.vColor[1] = m_pLight->m_pTransform->m_vLook.y;
	m_pMap->m_cbData.vColor[2] = m_pLight->m_pTransform->m_vLook.z;
	g_pImmediateContext->PSSetShaderResources(3, 1,
		m_pDepthMap->m_pRT->m_pSRV.GetAddressOf());
	m_pMap->Render(g_pImmediateContext);

	m_pShadowMinimap->m_pTransform->SetMatrix(NULL,NULL,NULL);

	m_pShadowMinimap->Update(g_pImmediateContext);
	m_pShadowMinimap->PreRender(g_pImmediateContext);
	m_pShadowMinimap->SettingPipeLine(g_pImmediateContext);

	g_pImmediateContext->PSSetShaderResources(0, 1,
		m_pDepthMap->m_pRT->m_pSRV.GetAddressOf());

	m_pShadowMinimap->myGraphics::Draw(g_pImmediateContext);

	return true;
}

bool Sample::Release()
{
	m_pDepthMap->Release();
	SAFE_DEL(m_pDepthMap);
	return true;
}