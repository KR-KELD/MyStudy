#include "Sample.h"
GAMERUN;

HRESULT Sample::CreateShadowCB()
{
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(cbShadowMatrix);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = &m_cbShadow;
	hr = g_pd3dDevice->CreateBuffer(&bd, &data, &m_pShadowCB);
	if (FAILED(hr)) return hr;
	return hr;
}

bool Sample::Init()
{
	CreateShadowCB();
	m_pPSShadow = StaticGraphics::LoadPixelShaderFile(
		g_pd3dDevice,
		L"../../data/shader/ModelPS.txt",
		"PSShadow");
	m_pPSShadowMap = StaticGraphics::LoadPixelShaderFile(
		g_pd3dDevice,
		L"../../data/shader/MapPS.txt",
		"PSShadow");

	m_pShadowMapRT = new myDxRT;
	m_pShadowMapRT->SetRenderTargetView();
	m_pShadowMapRT->SetDepthStencilView();
	m_pShadowMapRT->SetViewport();

	m_pShadowMinimap = new myMiniMap;
	g_ObjMgr.CreateObjComponent(L"ShadowMap", m_pShadowMinimap, OBJECT_SUB);
	m_pShadowMinimap->SetInfo(Vector3(-0.75f, 0.75f, 0.0f), 0.25f);
	m_pShadowMinimap->Create(L"BasisVS.txt", L"BasisPS.txt", L"");

	m_pLight = new myCamera;
	myGameObject* obj = g_CamMgr.CreateCameraObj(L"LightCamera", m_pLight);
	m_pLight->CreateViewMatrix({ 100,100, 0 }, { 0,0,0 });
	m_pLight->CreateProjMatrix(1.0f, 500.0f, PI4D, 1.0f);

	m_matShadowTex._11 = 0.5f; m_matShadowTex._22 = -0.5f;
	m_matShadowTex._41 = 0.5f; m_matShadowTex._42 = 0.5f;

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
	m_cbShadow.g_matShadow = m_pLight->m_pTransform->m_matView * m_pLight->m_pTransform->m_matProj * m_matShadowTex;

	int a = 0;
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
	if (m_pShadowMapRT->Begin())
	{
		m_pModelObj->m_pTransform->SetMatrix(NULL,
			&m_pLight->m_pTransform->m_matView,
			&m_pLight->m_pTransform->m_matProj);
		m_pModelObj->PreRender(g_pImmediateContext);
		g_pImmediateContext->PSSetShader(m_pPSShadow, NULL, 0);
		m_pModelObj->PostRender(g_pImmediateContext);

		m_pMap->m_pTransform->SetMatrix(NULL,
			&m_pLight->m_pTransform->m_matView,
			&m_pLight->m_pTransform->m_matProj);

		m_pMap->Update(g_pImmediateContext);
		m_pMap->PreRender(g_pImmediateContext);
		m_pMap->SettingPipeLine(g_pImmediateContext);
		g_pImmediateContext->PSSetShader(m_pPSShadowMap, NULL, 0);
		m_pMap->Draw(g_pImmediateContext);

		m_pShadowMapRT->End();
	}
	m_cbShadow.g_matShadow.Transpose();
	g_pImmediateContext->UpdateSubresource(m_pShadowCB, 0, NULL, &m_cbShadow, 0, 0);
	g_pImmediateContext->VSSetConstantBuffers(2, 1, &m_pShadowCB);

	g_pImmediateContext->RSSetState(myDxState::g_pRSSolid);
	g_pImmediateContext->PSSetSamplers(1, 1, &myDxState::g_pSSClampLinear);

	m_pModelObj->m_pTransform->SetMatrix(NULL,
		&g_pMainCamTransform->m_matView,
		&g_pMainCamTransform->m_matProj);
	m_pModelObj->PreRender(g_pImmediateContext);
	g_pImmediateContext->PSSetShaderResources(2, 1,
		m_pShadowMapRT->m_pSRV.GetAddressOf());
	m_pModelObj->PostRender(g_pImmediateContext);

	m_pMap->m_pTransform->SetMatrix(NULL,
		&g_pMainCamTransform->m_matView,
		&g_pMainCamTransform->m_matProj);
	m_pMap->Update(g_pImmediateContext);
	m_pMap->PreRender(g_pImmediateContext);
	g_pImmediateContext->PSSetShaderResources(3, 1,
		m_pShadowMapRT->m_pSRV.GetAddressOf());
	m_pMap->Render(g_pImmediateContext);

	m_pShadowMinimap->m_pTransform->SetMatrix(NULL,NULL,NULL);

	m_pShadowMinimap->Update(g_pImmediateContext);
	m_pShadowMinimap->PreRender(g_pImmediateContext);
	m_pShadowMinimap->SettingPipeLine(g_pImmediateContext);

	g_pImmediateContext->PSSetShaderResources(0, 1,
		m_pShadowMapRT->m_pSRV.GetAddressOf());

	m_pShadowMinimap->myGraphics::Draw(g_pImmediateContext);

	return true;
}

bool Sample::Release()
{
	if (m_pPSShadow) m_pPSShadow->Release();
	if (m_pPSShadowMap) m_pPSShadowMap->Release();

	m_pShadowMapRT->Release();
	SAFE_DEL(m_pShadowMapRT);
	m_pLight->Release();
	return true;
}