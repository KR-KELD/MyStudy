#include "Sample.h"
GAMERUN;

bool Sample::Init()
{
	g_FbxLoader.Init();
	//m_pFbxObj = g_FbxLoader.Load("../../data/object/Turret_Deploy1.fbx");
	//�׽�Ʈ ���� ����� ������ ��ġ���� ����
	//���븸 �̾ƾ� �ϴ��� ����



	m_pFbxObj = g_FbxLoader.Load("../../data/object/man.fbx");
	m_pModelObj = m_pFbxObj->m_pModelObject;
	m_pFbxObj->CuttingAnimScene(L"0", 0, 60);
	m_pFbxObj->CuttingAnimScene(L"1", 61, 91);
	m_pFbxObj->CuttingAnimScene(L"2", 92, 116);
	m_pFbxObj->CuttingAnimScene(L"3", 117, 167);
	m_pFbxObj->CuttingAnimScene(L"4", 168, 208);

	m_pFbxObj->CuttingAnimScene(L"5", 209, 239);
	m_pFbxObj->CuttingAnimScene(L"6", 240, 287);
	m_pFbxObj->CuttingAnimScene(L"7", 288, 319);
	m_pModelObj->m_pAnim->ChangeAnim(L"0");


	//m_pFbxObj->CuttingAnimScene(L"1", m_pFbxObj->m_AnimScene.iFirstFrame, m_pFbxObj->m_AnimScene.iLastFrame);
	//m_pModelObj->m_pAnim->ChangeAnim(L"1");

	return true;
}

bool Sample::Frame()
{
	m_pModelObj->Frame();
	if (g_Input.GetKey('0') == KEY_PUSH)
	{
		m_pModelObj->m_pAnim->ChangeAnim(L"0");
	}
	if (g_Input.GetKey('1') == KEY_PUSH)
	{
		m_pModelObj->m_pAnim->ChangeAnim(L"1");
	}
	if (g_Input.GetKey('2') == KEY_PUSH)
	{
		m_pModelObj->m_pAnim->ChangeAnim(L"2");
	}
	if (g_Input.GetKey('3') == KEY_PUSH)
	{
		m_pModelObj->m_pAnim->ChangeAnim(L"3");
	}
	if (g_Input.GetKey('4') == KEY_PUSH)
	{
		m_pModelObj->m_pAnim->ChangeAnim(L"4");
	}
	if (g_Input.GetKey('5') == KEY_PUSH)
	{
		m_pModelObj->m_pAnim->ChangeAnim(L"5");
	}
	if (g_Input.GetKey('6') == KEY_PUSH)
	{
		m_pModelObj->m_pAnim->ChangeAnim(L"6");
	}
	if (g_Input.GetKey('7') == KEY_PUSH)
	{
		m_pModelObj->m_pAnim->ChangeAnim(L"7");
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
