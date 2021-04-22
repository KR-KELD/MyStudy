#include "Sample.h"
GAMERUN;

bool Sample::Init()
{
	g_FbxLoader.Init();

	//테스트 오류 노드의 갯수가 일치하지 않음
	//뼈대만 뽑아야 하는지 질문

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


	m_pFbxObj = g_FbxLoader.Load("../../data/object/man.fbx");
	m_pModelObj = m_pFbxObj->m_pModelObject;
	m_pFbxObj->CuttingAnimScene(L"0", 0, 60);
	m_pFbxObj->CuttingAnimScene(L"1", 61, 91);
	//m_pFbxObj->CuttingAnimScene(L"2", 92, 116);
	//m_pFbxObj->CuttingAnimScene(L"3", 117, 167);
	//m_pFbxObj->CuttingAnimScene(L"4", 168, 208);

	//m_pFbxObj->CuttingAnimScene(L"5", 209, 239);
	//m_pFbxObj->CuttingAnimScene(L"6", 240, 287);
	//m_pFbxObj->CuttingAnimScene(L"7", 288, 319);
	m_pModelObj->m_pAnim->ChangeAnim(L"1");

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
		//오브젝트 띄우기
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
	//m_QuadTree.Render(g_pImmediateContext);

	//셀렉트노드 드로우
	m_Map->Update(g_pImmediateContext);
	m_Map->SettingPipeLine(g_pImmediateContext);
	g_pImmediateContext->IASetIndexBuffer(NULL, DXGI_FORMAT_R32_UINT, 0);
	for (myNode* pNode : m_SelectNodeList)
	{
		g_pImmediateContext->IASetIndexBuffer(pNode->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		g_pImmediateContext->DrawIndexed(pNode->m_IndexList.size(), 0, 0);

	}

	//최종적으로는 모든 함수는 각자의 Render에서 돌아가게끔 해야함
	//그걸 호출하는건 obj매니저에 있는 메인gameobject


	
	return true;
}

bool Sample::Release()
{
	g_FbxLoader.Release();
	return true;
}
