#include "Sample.h"
GAMERUN;

bool Sample::Init()
{
	g_FbxLoader.Init();

	//�׽�Ʈ ���� ����� ������ ��ġ���� ����
	//���븸 �̾ƾ� �ϴ��� ����

	m_Map = new myMap;
	g_ObjMgr.CreateObjComponent(L"Map", m_Map);
	myMapDesc desc;
	desc.iNumCols = 257;//m_Map->m_iNumCols;
	desc.iNumRows = 257;//m_Map->m_iNumRows;
	desc.fCellDistance = 10;
	desc.fScaleHeight = 10.0f;
	desc.szTexFile = L"../../data/castle.jpg";
	desc.szVS = L"../../data/shader/BasisVS.txt";
	desc.szPS = L"../../data/shader/BasisPS.txt";
	m_Map->CreateMap(desc);
	m_Map->m_isRender = false;
	m_QuadTree.CreateQuadTree(m_Map);

	m_pIns = new myInstanceGraphics;
	m_pInsObj = myGameObject::CreateComponentObj(m_pIns);
	m_pIns->Create(L"../../data/shader/InsVS.txt", L"../../data/shader/InsPS.txt",
		L"../../data/castle.jpg");
	m_pIns->CreateInstanceBuffer();
	m_iNum = 0;


	return true;
}

bool Sample::Frame()
{
	m_SelectNodeList.clear();
	if (g_Input.GetKey(VK_RBUTTON) == KEY_PUSH)
	{
		m_Mouse.ScreenToRay();
		for (int i = 0; i < m_QuadTree.m_LeafNodeList.size(); i++)
		{
			myNode* pNode = m_QuadTree.m_LeafNodeList[i];
			if (myCollision::IntersectRayToBox(m_Mouse.m_myRay, pNode->m_myBox/*,&m_Mouse.m_vIntersectionPos*/))
			{
				m_SelectNodeList.push_back(pNode);
			}
		}
	}

	Vector3 vPick;
	float fMaxDist = 99999.0f;
	bool bUpdate = false;

	for (myNode* pNode : m_SelectNodeList)
	{
		if (m_Mouse.PickingFace(pNode))
		{
			float fDist = (m_Mouse.m_vIntersectionPos - m_Mouse.m_myRay.vOrigin).Length();
			if (fDist < fMaxDist)
			{
				vPick = m_Mouse.m_vIntersectionPos;
				fMaxDist = fDist;
				bUpdate = true;
			}
		}
	}
	if (bUpdate)
	{
		//������Ʈ ����
		//myModelInstance ins;
		//ins.vPos = vPick;
		//ins.fTick = 0.0f;
		//m_ModelList.push_back(ins);
		m_pIns->m_InstanceList[m_iNum].matWorld._41 = vPick.x;
		m_pIns->m_InstanceList[m_iNum].matWorld._42 = vPick.y;
		m_pIns->m_InstanceList[m_iNum].matWorld._43 = vPick.z;
		m_iNum++;
		if (m_iNum > m_pIns->m_iNumIntance) m_iNum = 0;
	}


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
	//if (g_Input.GetKey('3') == KEY_PUSH)
	//{
	//	m_pModelObj->m_pAnim->ChangeAnim(L"3");
	//}
	//if (g_Input.GetKey('4') == KEY_PUSH)
	//{
	//	m_pModelObj->m_pAnim->ChangeAnim(L"4");
	//}
	//if (g_Input.GetKey('5') == KEY_PUSH)
	//{
	//	m_pModelObj->m_pAnim->ChangeAnim(L"5");
	//}
	//if (g_Input.GetKey('6') == KEY_PUSH)
	//{
	//	m_pModelObj->m_pAnim->ChangeAnim(L"6");
	//}
	//if (g_Input.GetKey('7') == KEY_PUSH)
	//{
	//	m_pModelObj->m_pAnim->ChangeAnim(L"7");
	//}

	//for (int i=0;i< m_ModelList.size();i++)
	//{

	//}
	myBaseInstance* pInstances = NULL;
	D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
	if (SUCCEEDED(g_pImmediateContext->Map((ID3D11Resource*)m_pIns->m_pInstanceBuffer.Get(),
		0, D3D11_MAP_WRITE_DISCARD, 0, &MappedFaceDest)))
	{
		pInstances = (myBaseInstance*)MappedFaceDest.pData;
		memcpy(pInstances, &m_pIns->m_InstanceList.at(0), sizeof(myBaseInstance) *m_pIns->m_InstanceList.size());
		g_pImmediateContext->Unmap(m_pIns->m_pInstanceBuffer.Get(), 0);
	}

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


	m_Map->m_pTransform->SetMatrix(NULL,
		&g_pMainCamTransform->m_matView,
		&g_pMainCamTransform->m_matProj);
	m_QuadTree.Render(g_pImmediateContext);

	m_pInsObj->m_pTransform->SetMatrix(NULL,
		&g_pMainCamTransform->m_matView,
		&g_pMainCamTransform->m_matProj);
	m_pIns->InstancingRender(g_pImmediateContext, m_iNum);
	//m_pIns->Render(g_pImmediateContext);

	//����Ʈ��� ��ο�
	//m_Map->Update(g_pImmediateContext);
	//m_Map->SettingPipeLine(g_pImmediateContext);
	//g_pImmediateContext->IASetIndexBuffer(NULL, DXGI_FORMAT_R32_UINT, 0);
	//for (myNode* pNode : m_SelectNodeList)
	//{
	//	g_pImmediateContext->IASetIndexBuffer(pNode->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	//	g_pImmediateContext->DrawIndexed(pNode->m_IndexList.size(), 0, 0);

	//}

	//���������δ� ��� �Լ��� ������ Render���� ���ư��Բ� �ؾ���
	//�װ� ȣ���ϴ°� obj�Ŵ����� �ִ� ����gameobject


	
	return true;
}

bool Sample::Release()
{
	g_FbxLoader.Release();
	m_pInsObj->Release();
	SAFE_DELETE(m_pInsObj);
	return true;
}
