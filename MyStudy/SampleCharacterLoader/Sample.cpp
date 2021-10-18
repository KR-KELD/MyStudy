#include "Sample.h"
GAMERUN;

bool Sample::Init()
{
	
	//테스트 오류 노드의 갯수가 일치하지 않음
	//뼈대만 뽑아야 하는지 질문


	m_pFbxObj = g_FbxLoader.Load("../../data/object/man.fbx");
	

	m_pModelObj = m_pFbxObj->m_pModelObject;
	m_pModelObj->m_pNormalTex = g_TextureMgr.Load(L"../../data/object/test_normal_map.bmp");
	//m_pModelObj->m_pNormalTex = g_TextureMgr.m_pWhiteTexture;
	//m_pFbxObj->CuttingAnimScene(L"0", m_pFbxObj->m_AnimScene.iFirstFrame, m_pFbxObj->m_AnimScene.iLastFrame);
	m_pFbxObj->CuttingAnimScene(L"0", 0, 60);
	m_pFbxObj->CuttingAnimScene(L"1", 61, 91);
	m_pFbxObj->CuttingAnimScene(L"2", 92, 116);
	m_pFbxObj->CuttingAnimScene(L"3", 117, 167);
	m_pFbxObj->CuttingAnimScene(L"4", 168, 208);

	m_pFbxObj->CuttingAnimScene(L"5", 209, 239);
	m_pFbxObj->CuttingAnimScene(L"6", 240, 287);
	m_pFbxObj->CuttingAnimScene(L"7", 288, 319);
	m_pModelObj->ChangeAnim(L"0");

	return true;
}

bool Sample::Frame()
{

	m_pModelObj->Frame();
	if (g_Input.GetKey('0') == KEY_PUSH)
	{
		m_pModelObj->ChangeAnim(L"0");
	}
	if (g_Input.GetKey('1') == KEY_PUSH)
	{
		m_pModelObj->ChangeAnim(L"1");
	}
	if (g_Input.GetKey('2') == KEY_PUSH)
	{
		m_pModelObj->ChangeAnim(L"2");
	}
	if (g_Input.GetKey('3') == KEY_PUSH)
	{
		m_pModelObj->ChangeAnim(L"3");
	}
	if (g_Input.GetKey('4') == KEY_PUSH)
	{
		m_pModelObj->ChangeAnim(L"4");
	}
	if (g_Input.GetKey('5') == KEY_PUSH)
	{
		m_pModelObj->ChangeAnim(L"5");
	}
	if (g_Input.GetKey('6') == KEY_PUSH)
	{
		m_pModelObj->ChangeAnim(L"6");
	}
	if (g_Input.GetKey('7') == KEY_PUSH)
	{
		m_pModelObj->ChangeAnim(L"7");
	}

	//for (int i=0;i< m_ModelList.size();i++)
	//{

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
