#include "Sample.h"
GAMERUN;

bool Sample::Init()
{
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
	desc.fCellDistance = 4;
	desc.fScaleHeight = 10.0f;
	desc.szTexFile = L"main.png";
	desc.szVS = L"MapVS.txt";
	desc.szPS = L"MapTest.txt";
	m_pMap->CreateMap(desc);
	m_pMap->SetMapCBData(8, 8, 4, 1);

	//pApp->m_Sample.m_QuadTree.CreateQuadTree(pApp->m_Sample.m_pMap);
	//pApp->m_Sample.m_isCreate = true;
	//pApp->m_Sample.m_pMap->SetMapCBData(iNumCell, iNumTile, m_iCellSize, iSpaceDivision);
	return true;
}

bool Sample::Frame()
{
	m_pModelObj->Frame();

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
	return true;
}