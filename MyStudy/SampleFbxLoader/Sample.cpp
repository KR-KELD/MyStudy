#include "Sample.h"
GAMERUN;

bool Sample::Init()
{
	g_FbxLoader.Init();
	//m_pFbxObj = g_FbxLoader.Load("../../data/object/Turret_Deploy1.fbx");
	m_pFbxObj = g_FbxLoader.Load("../../data/object/Scifi_Model_L2_all_in_one.fbx");
	m_pModelObj = m_pFbxObj->m_pModelObject;
	m_pFbxObj->CuttingAnimScene(L"1", 0, 30);
	m_pFbxObj->CuttingAnimScene(L"2", 30, 60);
	m_pModelObj->m_pAnim->ChangeAnim(L"1");
	//fbx������Ʈ�� ������� gameobject �����͸� ä���ش�

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
	//���������δ� ��� �Լ��� ������ Render���� ���ư��Բ� �ؾ���
	//�װ� ȣ���ϴ°� obj�Ŵ����� �ִ� ����gameobject
	m_pModelObj->Render();
	return true;
}

bool Sample::Release()
{
	g_FbxLoader.Release();
	return true;
}
