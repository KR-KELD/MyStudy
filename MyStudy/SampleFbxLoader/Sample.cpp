#include "Sample.h"
GAMERUN;

bool Sample::Init()
{
	g_FbxLoader.Init();
	//m_pFbxObj = g_FbxLoader.Load("../../data/object/Turret_Deploy1.fbx");
	//�׽�Ʈ ���� ����� ������ ��ġ���� ����
	//���븸 �̾ƾ� �ϴ��� ����
	//m_pFbxObj->CuttingAnimScene(L"1", m_pFbxObj->m_AnimScene.iFirstFrame, m_pFbxObj->m_AnimScene.iLastFrame);



	//m_pFbxObj = g_FbxLoader.Load("../../data/object/man.fbx");
	//m_pModelObj = m_pFbxObj->m_pModelObject;
	//m_pFbxObj->CuttingAnimScene(L"0", 0, 60);
	//m_pFbxObj->CuttingAnimScene(L"1", 61, 91);
	//m_pFbxObj->CuttingAnimScene(L"2", 92, 116);
	//m_pModelObj->m_pAnim->ChangeAnim(L"0");

	return true;
}

bool Sample::Frame()
{

	//m_pModelObj->Frame();
	//if (g_Input.GetKey('0') == KEY_PUSH)
	//{
	//	m_pModelObj->m_pAnim->ChangeAnim(L"0");
	//}
	//if (g_Input.GetKey('1') == KEY_PUSH)
	//{
	//	m_pModelObj->m_pAnim->ChangeAnim(L"1");
	//}
	//if (g_Input.GetKey('2') == KEY_PUSH)
	//{
	//	m_pModelObj->m_pAnim->ChangeAnim(L"2");
	//}
	return true;
}

bool Sample::Render()
{

	//���������δ� ��� �Լ��� ������ Render���� ���ư��Բ� �ؾ���
	//�װ� ȣ���ϴ°� obj�Ŵ����� �ִ� ����gameobject
	//m_pModelObj->Render();
	return true;
}

bool Sample::Release()
{
	g_FbxLoader.Release();
	return true;
}
