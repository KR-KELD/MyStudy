#include "Sample.h"
GAMERUN;

bool Sample::Init()
{
	g_FbxLoader.Init();
	//m_pFbxObj = g_FbxLoader.Load("../../data/object/Turret_Deploy1.fbx");
	//�׽�Ʈ ���� ����� ������ ��ġ���� ����
	//���븸 �̾ƾ� �ϴ��� ����
	//m_pFbxObj = g_FbxLoader.Load("../../data/object/man.fbx");
	//m_pModelObj = m_pFbxObj->m_pModelObject;
	//m_pFbxObj->CuttingAnimScene(L"1", m_pFbxObj->m_AnimScene.iFirstFrame, m_pFbxObj->m_AnimScene.iLastFrame);
	//m_pFbxObj->CuttingAnimScene(L"1", 62,91);
	//m_pFbxObj->CuttingAnimScene(L"2", 92, 116);
	//m_pModelObj->m_pAnim->ChangeAnim(L"1");
	//fbx������Ʈ�� ������� gameobject �����͸� ä���ش�

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
	//���������δ� ��� �Լ��� ������ Render���� ���ư��Բ� �ؾ���
	//�װ� ȣ���ϴ°� obj�Ŵ����� �ִ� ����gameobject
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
