#include "Sample.h"
GAMERUN;

bool Sample::Init()
{
	g_FbxLoader.Init();
	m_pFbxObj = g_FbxLoader.Load("../../data/object/Turret_Deploy1.fbx");
	//테스트 오류 노드의 갯수가 일치하지 않음
	//뼈대만 뽑아야 하는지 질문
	//m_pFbxObj2 = g_FbxLoader.Load("../../data/object/Scifi_Model_L2_all_in_one.fbx");
	//m_pFbxObj = g_FbxLoader.Load("../../data/object/man.fbx");
	m_pModelObj = m_pFbxObj->m_pModelObject;
	m_pFbxObj->CuttingAnimScene(L"1", m_pFbxObj->m_AnimScene.iFirstFrame, m_pFbxObj->m_AnimScene.iLastFrame);
	//m_pModelObj->SetAnim(L"2", m_pFbxObj2->m_AnimScene, m_pFbxObj2->m_pModelObject->m_myNodeList);
	m_pModelObj->m_pAnim->ChangeAnim(L"1");
	//fbx오브젝트를 기반으로 gameobject 데이터를 채워준다

	return true;
}

bool Sample::Frame()
{
	m_pModelObj->Frame();
	if (g_Input.GetKey('1') == KEY_PUSH)
	{
		m_pModelObj->m_pAnim->ChangeAnim(L"1");
	}
	if (g_Input.GetKey('2') == KEY_PUSH)
	{
		m_pModelObj->m_pAnim->ChangeAnim(L"2");
	}
	return true;
}

bool Sample::Render()
{
	//최종적으로는 모든 함수는 각자의 Render에서 돌아가게끔 해야함
	//그걸 호출하는건 obj매니저에 있는 메인gameobject
	m_pModelObj->Render();
	return true;
}

bool Sample::Release()
{
	g_FbxLoader.Release();
	return true;
}
