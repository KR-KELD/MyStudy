#include "Sample.h"
GAMERUN;

bool Sample::Init()
{
	g_FbxLoader.Init();
	m_pFbxObj = g_FbxLoader.Load("../../data/object/Turret_Deploy1.fbx");
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
	g_pImmediateContext->VSSetConstantBuffers(1, 1, m_pModelObj->m_pBoneBuffer.GetAddressOf());
	for (auto p: m_pModelObj->m_myNodeList)
	{
		myModelGraphics* pGraphics = p->GetComponent<myModelGraphics>();
		//p->m_iTick += g_fSecondPerFrame * p->m_p
		pGraphics->m_pTransform->SetMatrix(NULL,
			&g_pMainCamTransform->m_matView,
			&g_pMainCamTransform->m_matProj);
		pGraphics->m_cbData.vColor[0] = g_pMainCamTransform->m_vLook.x;
		pGraphics->m_cbData.vColor[1] = g_pMainCamTransform->m_vLook.y;
		pGraphics->m_cbData.vColor[2] = g_pMainCamTransform->m_vLook.z;
		pGraphics->Render();
	}
	
	return true;
}

bool Sample::Release()
{
	g_FbxLoader.Release();
	return true;
}
