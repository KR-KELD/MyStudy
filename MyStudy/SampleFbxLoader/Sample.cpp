#include "Sample.h"
GAMERUN;

bool Sample::Init()
{
	g_FbxLoader.Init();
	//m_pFbxObj = g_FbxLoader.Load("../../data/object/Turret_Deploy1.fbx");
	//테스트 오류 노드의 갯수가 일치하지 않음
	//뼈대만 뽑아야 하는지 질문
	//m_pFbxObj = g_FbxLoader.Load("../../data/object/man.fbx");
	//m_pModelObj = m_pFbxObj->m_pModelObject;
	//m_pFbxObj->CuttingAnimScene(L"1", m_pFbxObj->m_AnimScene.iFirstFrame, m_pFbxObj->m_AnimScene.iLastFrame);
	//m_pFbxObj->CuttingAnimScene(L"1", 62,91);
	//m_pFbxObj->CuttingAnimScene(L"2", 92, 116);
	//m_pModelObj->m_pAnim->ChangeAnim(L"1");
	//fbx오브젝트를 기반으로 gameobject 데이터를 채워준다

	m_pSkyBoxObj = myGameObject::CreateComponentObj(new mySkyBox);
	m_pSkyBox = m_pSkyBoxObj->GetComponent<mySkyBox>();
	m_pSkyBox->Create(L"../../data/shader/VS1.txt", L"../../data/shader/SkyBox.txt", L"");

	return true;
}

bool Sample::Frame()
{
	m_pSkyBoxObj->Frame();
	//m_pModelObj->Frame();
	//if (g_Input.GetKey('1') == KEY_PUSH)
	//{
	//	m_pModelObj->m_pAnim->ChangeAnim(L"1",true,1.5f);
	//}
	//if (g_Input.GetKey('2') == KEY_PUSH)
	//{
	//	m_pModelObj->m_pAnim->ChangeAnim(L"2", true, 1.5f);
	//}
	return true;
}

bool Sample::Render()
{
	m_pSkyBoxObj->m_pTransform->SetMatrix(NULL,
		&g_pMainCamTransform->m_matView,
		&g_pMainCamTransform->m_matProj);
	m_pSkyBoxObj->Render();
	//최종적으로는 모든 함수는 각자의 Render에서 돌아가게끔 해야함
	//그걸 호출하는건 obj매니저에 있는 메인gameobject
	//m_pModelObj->Render();
	return true;
}

bool Sample::Release()
{
	m_pSkyBoxObj->Release();
	delete m_pSkyBoxObj;
	m_pSkyBoxObj = nullptr;

	g_FbxLoader.Release();
	return true;
}
