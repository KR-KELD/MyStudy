#include "Sample.h"
GAMERUN;

bool Sample::Init()
{
	g_FbxLoader.Init();
	//m_pFbxObj = g_FbxLoader.Load("../../data/object/Turret_Deploy1.fbx");
	m_pFbxObj = g_FbxLoader.Load("../../data/object/Scifi_Model_L2_all_in_one.fbx");
	m_pModelObj = m_pFbxObj->m_pModelObject;
	//fbx������Ʈ�� ������� gameobject �����͸� ä���ش�

	return true;
}

bool Sample::Frame()
{
	m_pModelObj->Frame();
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
