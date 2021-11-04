#include "Sample.h"
GAMERUN;

bool Sample::Init()
{
	m_pMap = new myMap;
	g_ObjMgr.CreateObjComponent(L"Map", m_pMap, OBJECT_SUB);

	myMapDesc desc;
	desc.iNumCols = 65;
	desc.iNumRows = 65;
	desc.fCellDistance = 4;
	desc.fScaleHeight = 10.0f;
	desc.szTexFile = L"main.png";
	desc.szVS = L"MapVS.txt";
	desc.szPS = L"MapTestPS.txt";
	m_pMap->CreateMap(desc);
	m_pMap->SetMapCBData(8, 8, 4, 1);

	m_pFbxObj = g_FbxLoader.Load("../../data/object/man.fbx");

	myCamera* pBackCamera = new myCamera;
	myGameObject* obj = g_CamMgr.CreateCameraObj(L"BackCamera", pBackCamera);
	pBackCamera->CreateViewMatrix({ 0,10,-10 }, { 0,0,0 });
	float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	pBackCamera->CreateProjMatrix(1, 1000, PI2D, fAspect);
	pBackCamera->CreateFrustum();

	m_pModelObj = m_pFbxObj->m_pModelObject;
	m_pModelObj->m_pTransform->m_qRot.y = 135.0f;
	//m_pModelObj->m_pNormalTex = g_TextureMgr.Load(L"../../data/object/test_normal_map.bmp");
	m_pModelObj->m_pNormalTex = g_TextureMgr.Load(L"../../data/object/base_normal_map.bmp");
	//m_pModelObj->m_pNormalTex = g_TextureMgr.m_pWhiteTexture;
	m_pFbxObj->CuttingAnimScene(L"0", m_pFbxObj->m_AnimScene.iFirstFrame, m_pFbxObj->m_AnimScene.iLastFrame);
	m_pFbxObj->CuttingAnimScene(L"1", 0, 60);
	m_pFbxObj->CuttingAnimScene(L"2", 61, 91);
	m_pFbxObj->CuttingAnimScene(L"3", 92, 116);
	m_pFbxObj->CuttingAnimScene(L"4", 117, 167);
	m_pFbxObj->CuttingAnimScene(L"5", 168, 208);

	m_pFbxObj->CuttingAnimScene(L"6", 209, 239);
	m_pFbxObj->CuttingAnimScene(L"7", 240, 287);
	m_pFbxObj->CuttingAnimScene(L"8", 288, 319);
	m_pModelObj->ChangeAnim(L"1");
	m_pModelObj->InsertComponent(new myFPSController);

	g_CamMgr.SetMainCamera(L"BackCamera");

	g_pMainCamTransform->m_vTarget = m_pModelObj->m_pTransform->m_vPos;
	m_pModelObj->m_pTransform->m_fSpeed = 50.0f;
	g_pMainCamTransform->m_vTarget = m_pModelObj->m_pTransform->m_vPos;
	g_pMainCamTransform->m_vPos = Vector3(
		g_pMainCamTransform->m_vTarget.x,
		g_pMainCamTransform->m_vTarget.y + 100.0f,
		g_pMainCamTransform->m_vTarget.z - 60.0f);
	return true;
}

bool Sample::Frame()
{
	g_pMainCamTransform->m_vTarget = m_pModelObj->m_pTransform->m_vPos;
	g_pMainCamTransform->m_vPos = Vector3(
		g_pMainCamTransform->m_vTarget.x,
		g_pMainCamTransform->m_vTarget.y + 100.0f,
		g_pMainCamTransform->m_vTarget.z - 60.0f);
	m_pModelObj->Frame();

	myFPSController* PlayerController = m_pModelObj->GetComponent<myFPSController>();
	
	if (PlayerController->m_eMoveState == myEMoveState::MOVE_STOP)
	{
		m_pModelObj->ChangeAnim(L"1");
	}
	else
	{
		if (PlayerController->m_bRun)
		{
			m_pModelObj->ChangeAnim(L"3");
		}
		else
		{
			m_pModelObj->ChangeAnim(L"2");
		}
	}

	//if (g_Input.GetKey(VK_F1) == KEY_PUSH)
	//{
	//	m_pModelObj->ChangeAnim(L"1");
	//}
	//if (g_Input.GetKey('0') == KEY_PUSH)
	//{
	//	m_pModelObj->ChangeAnim(L"1");
	//}
	//if (g_Input.GetKey('1') == KEY_PUSH)
	//{
	//	m_pModelObj->ChangeAnim(L"2");
	//}
	//if (g_Input.GetKey('2') == KEY_PUSH)
	//{
	//	m_pModelObj->ChangeAnim(L"3");
	//}
	//if (g_Input.GetKey('3') == KEY_PUSH)
	//{
	//	m_pModelObj->ChangeAnim(L"4");
	//}
	//if (g_Input.GetKey('4') == KEY_PUSH)
	//{
	//	m_pModelObj->ChangeAnim(L"5");
	//}
	//if (g_Input.GetKey('5') == KEY_PUSH)
	//{
	//	m_pModelObj->ChangeAnim(L"6");
	//}
	//if (g_Input.GetKey('6') == KEY_PUSH)
	//{
	//	m_pModelObj->ChangeAnim(L"7");
	//}
	//if (g_Input.GetKey('7') == KEY_PUSH)
	//{
	//	m_pModelObj->ChangeAnim(L"8");
	//}

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
	m_pModelObj->m_pTransform->SetMatrix(NULL,
		&g_pMainCamTransform->m_matView,
		&g_pMainCamTransform->m_matProj);
	m_pModelObj->Render(g_pImmediateContext);
	g_CamMgr.m_pMainCamera->FrameFrustum();

	m_pMap->m_pTransform->SetMatrix(NULL,
		&g_pMainCamTransform->m_matView,
		&g_pMainCamTransform->m_matProj);

	m_pMap->Update(g_pImmediateContext);
	m_pMap->PreRender(g_pImmediateContext);
	m_pMap->Render(g_pImmediateContext);

	/*for (int i = 0; i < m_ModelList.size(); i++)
	{
		m_pModelObj->m_pAnim->m_fTick = m_ModelList[i].fTick;
		m_pModelObj->Frame();
		m_ModelList[i].fTick = m_pModelObj->m_pAnim->m_fTick;
		m_pModelObj->m_pTransform->m_vPos = m_ModelList[i].vPos;
		m_pModelObj->Render();
	}*/


	return true;
}

bool Sample::Release()
{
	return true;
}
