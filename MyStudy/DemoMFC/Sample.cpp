#include "pch.h"
#include "Sample.h"

bool Sample::Init()
{
	m_Map = new myMap;
	g_ObjMgr.CreateObjComponent(L"Map", m_Map);
	myMapDesc desc;
	int iNumCell = 2;
	m_QuadTree.m_iMaxdepth = 2;
	int iNumTile = pow(2, m_QuadTree.m_iMaxdepth);
	desc.iNumCols = iNumCell * iNumTile + 1;//m_Map->m_iNumCols;
	desc.iNumRows = iNumCell * iNumTile + 1;//m_Map->m_iNumRows;
	desc.fCellDistance = 3;
	desc.fScaleHeight = 10.0f;
	desc.szTexFile = L"../../data/castle.jpg";
	desc.szVS = L"../../data/shader/BasisVS.txt";
	desc.szPS = L"../../data/shader/BasisPS.txt";
	m_Map->CreateMap(desc);
	m_Map->m_isRender = false;
	m_QuadTree.CreateQuadTree(m_Map);

	return true;
}

bool Sample::Frame()
{
	//m_SelectNodeList.clear();
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

	//Vector3 vPick;
	//float fMaxDist = 99999.0f;
	//bool bUpdate = false;

	//for (myNode* pNode : m_SelectNodeList)
	//{
	//	if (m_Mouse.PickingFace(pNode))
	//	{
	//		float fDist = (m_Mouse.m_vIntersectionPos - m_Mouse.m_myRay.vOrigin).Length();
	//		if (fDist < fMaxDist)
	//		{
	//			vPick = m_Mouse.m_vIntersectionPos;
	//			fMaxDist = fDist;
	//			bUpdate = true;
	//		}
	//	}
	//}
	//if (bUpdate)
	//{
	//	//오브젝트 띄우기
	//	//myModelInstance ins;
	//	//ins.vPos = vPick;
	//	//ins.fTick = 0.0f;
	//	//m_ModelList.push_back(ins);
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

	return true;
}

bool Sample::Release()
{
	return true;
}
