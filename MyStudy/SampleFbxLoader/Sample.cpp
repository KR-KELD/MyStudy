#include "Sample.h"
GAMERUN;

bool Sample::Init()
{
	g_FbxLoader.Init();
	//m_pFbxObj = g_FbxLoader.Load("../../data/object/Turret_Deploy1.fbx");
	m_pFbxObj = g_FbxLoader.Load("../../data/object/Scifi_Model_L2_all_in_one.fbx");
	m_pModelObj = m_pFbxObj->m_pModelObject;
	//fbx오브젝트를 기반으로 gameobject 데이터를 채워준다

	return true;
}

bool Sample::Frame()
{
	m_pModelObj->Frame();
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
