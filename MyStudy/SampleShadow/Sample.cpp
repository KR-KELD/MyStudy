#include "Sample.h"
GAMERUN;

bool Sample::Init()
{
	m_pLight = make_shared<myCamera>();

	//m_pLight = new myCamera;
	//myGameObject* obj = g_CamMgr.CreateCameraObj(L"LightCamera", m_pLight);
	m_pLight->CreateViewMatrix({ 100,100, 0 }, { 0,0,0 });
	float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	m_pLight->CreateProjMatrix(1, 1000, PI2D, fAspect);


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
	desc.fCellDistance = 10;
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

	//m_pLight->m_pTransform->RightMovement(1.0f);
	//m_pLight->Frame();
	return true;
}

bool Sample::PreFrame()
{
	m_pModelObj->PreFrame();
	return true;
}

bool Sample::Render()
{
	m_pModelObj->m_pTransform->SetMatrix(NULL,
		&g_pMainCamTransform->m_matView,
		&g_pMainCamTransform->m_matProj);
	m_pModelObj->Render(g_pImmediateContext);

	m_pMap->m_pTransform->SetMatrix(NULL,
		&g_pMainCamTransform->m_matView,
		&g_pMainCamTransform->m_matProj);

	m_pMap->Update(g_pImmediateContext);
	m_pMap->PreRender(g_pImmediateContext);
	m_pMap->Render(g_pImmediateContext);

	return true;
}

bool Sample::Release()
{
	m_pLight->Release();
	return true;
}