#include "myQuadTree.h"
#include "myObjManager.h"
#include "myCameraManager.h"

bool myQuadTree::CreateQuadTree(myMap* pMap)
{
	m_pMap = pMap;
	m_pRootNode = CreateNode(nullptr, 0,
		m_pMap->m_iNumCols - 1,
		m_pMap->m_iNumVertices - 1 - (m_pMap->m_iNumCols - 1),
		m_pMap->m_iNumVertices - 1);

	Partition(m_pRootNode);



	m_pCullingCamera = g_CamMgr.m_pMainCamera;

	m_pDepthMap = new myDepthMap;
	m_pDepthMap->Init();

	m_pLight = new myCamera;
	myGameObject* obj = g_CamMgr.CreateCameraObj(L"LightCamera", m_pLight);
	m_pLight->CreateViewMatrix({ 400,300, 0 }, { 0,0,0 });

	float fWidthLength = m_pMap->m_fCellDistance*m_pMap->m_iNumCols*
		m_pMap->m_fCellDistance*m_pMap->m_iNumCols;
	float fHeightLength = m_pMap->m_fCellDistance*m_pMap->m_iNumRows*
		m_pMap->m_fCellDistance*m_pMap->m_iNumRows;
	float fMaxViewDistance = sqrt(fWidthLength + fHeightLength);

	m_pLight->m_pTransform->m_matProj = Matrix::CreateOrthographicOffCenter(
		-fMaxViewDistance / 2,
		fMaxViewDistance / 2,
		-fMaxViewDistance / 2,
		fMaxViewDistance / 2,
		1.0f, 1000.0f);

	m_pQuadTreeLine = new myShapeLine;
	g_RunGameObject.InsertComponent(m_pQuadTreeLine);
	m_pQuadTreeLine->Init();
	if (!m_pQuadTreeLine->Create(L"BasisVS.txt", L"BasisPS.txt", L""))
	{
		return false;
	}

	return true;
}

bool myQuadTree::Partition(myNode* pParentNode)
{
	if (pParentNode->m_CornerIndexList[1] - pParentNode->m_CornerIndexList[0] <= 1)
	{
		pParentNode->m_iIndex = m_LeafNodeList.size();
		m_LeafNodeList.push_back(pParentNode);
		pParentNode->m_isLeaf = true;
		return false;
	}
	if (pParentNode->m_iDepth + 1 > m_iMaxdepth)
	{
		pParentNode->m_iIndex = m_LeafNodeList.size();
		m_LeafNodeList.push_back(pParentNode);
		pParentNode->m_isLeaf = true;
		return false;
	}
	//크기제한

	DWORD CT, CL, CC, CR, CB;
	CT = (pParentNode->m_CornerIndexList[0] + pParentNode->m_CornerIndexList[1]) / 2;
	CL = (pParentNode->m_CornerIndexList[0] + pParentNode->m_CornerIndexList[2]) / 2;
	CR = (pParentNode->m_CornerIndexList[1] + pParentNode->m_CornerIndexList[3]) / 2;
	CB = (pParentNode->m_CornerIndexList[2] + pParentNode->m_CornerIndexList[3]) / 2;
	CC = (CT + CB) / 2;
	pParentNode->m_ChildList.resize(4);
	pParentNode->m_ChildList[0] = CreateNode(pParentNode, pParentNode->m_CornerIndexList[0], CT, CL, CC);
	pParentNode->m_ChildList[1] = CreateNode(pParentNode, CT, pParentNode->m_CornerIndexList[1], CC, CR);
	pParentNode->m_ChildList[2] = CreateNode(pParentNode, CL, CC, pParentNode->m_CornerIndexList[2], CB);
	pParentNode->m_ChildList[3] = CreateNode(pParentNode, CC, CR, CB, pParentNode->m_CornerIndexList[3]);

	Partition(pParentNode->m_ChildList[0]);
	Partition(pParentNode->m_ChildList[1]);
	Partition(pParentNode->m_ChildList[2]);
	Partition(pParentNode->m_ChildList[3]);
	return true;
}

//0 31 4032 4095
bool myQuadTree::Release()
{
	SAFE_DEL(m_pRootNode);
	return true;
}

bool myQuadTree::PreFrame()
{
	return false;
}

bool myQuadTree::Frame()
{
	Matrix Trans = m_pLight->m_pTransform->m_matWorld * Matrix::CreateRotationY(PI4D * g_fSecondPerFrame);
	m_pLight->m_pTransform->m_vPos = Vector3::Transform(m_pLight->m_pTransform->m_vPos, Trans);

	m_pLight->Frame();
	m_pDepthMap->m_cbData.g_matShadow = m_pLight->m_pTransform->m_matView *
		m_pLight->m_pTransform->m_matProj * m_pDepthMap->m_matShadowTex;
		m_pDepthMap->m_cbData.g_matShadow = m_pDepthMap->m_cbData.g_matShadow.Transpose();
	return false;
}

bool myQuadTree::Render(ID3D11DeviceContext*	pd3dContext)
{
	CullingObject();
	m_pMap->m_pTransform->SetMatrix(NULL,
		&g_pMainCamTransform->m_matView,
		&g_pMainCamTransform->m_matProj);
	m_pMap->Update(g_pImmediateContext);
	m_pMap->PreRender(pd3dContext);
	m_pMap->SettingPipeLine(pd3dContext);
	pd3dContext->IASetIndexBuffer(NULL, DXGI_FORMAT_R32_UINT, 0);
	DrawCulling(pd3dContext);

	DrawObject(pd3dContext);
	return true;
}

bool myQuadTree::DepthRender(ID3D11DeviceContext * pd3dContext)
{
	ApplyDSS(pd3dContext, myDxState::g_pDSSDepthEnable);
	ApplyRS(pd3dContext, myDxState::g_pRSSlopeScaledDepthBias);
	if (m_pDepthMap->m_pRT->Begin())
	{
		m_pMap->m_pTransform->SetMatrix(NULL,
			&m_pLight->m_pTransform->m_matView,
			&m_pLight->m_pTransform->m_matProj);
		m_pMap->m_isShadowRender = true;
		m_pMap->m_cbData.vColor[0] = m_pLight->m_pTransform->m_vLook.x;
		m_pMap->m_cbData.vColor[1] = m_pLight->m_pTransform->m_vLook.y;
		m_pMap->m_cbData.vColor[2] = m_pLight->m_pTransform->m_vLook.z;
		m_pMap->Update(g_pImmediateContext);
		m_pMap->PreRender(pd3dContext);
		m_pMap->SettingPipeLine(g_pImmediateContext);
		pd3dContext->IASetIndexBuffer(NULL, DXGI_FORMAT_R32_UINT, 0);

		DrawCulling(pd3dContext);
		m_pMap->m_isShadowRender = false;

		for (myNode* pNode : m_DrawNodeList)
		{
			for (int i = 0; i < pNode->m_ObjectList.size(); i++)
			{
				if (!pNode->m_ObjectList[i]->isActive) continue;
				if (!pNode->m_ObjectList[i]->isRender) continue;
				myGameObject* pObj = g_ObjMgr.m_ObjectList[pNode->m_ObjectList[i]->iID];
				myModelObject* pModel = (myModelObject*)pObj;
				if (pModel != nullptr)
				{
					pModel->m_pGraphics->m_cbData.vColor[0] = m_pLight->m_pTransform->m_vLook.x;
					pModel->m_pGraphics->m_cbData.vColor[1] = m_pLight->m_pTransform->m_vLook.y;
					pModel->m_pGraphics->m_cbData.vColor[2] = m_pLight->m_pTransform->m_vLook.z;
					pModel->m_pGraphics->m_isShadowRender = true;
				}
				pObj->m_pTransform->m_vPos = pNode->m_ObjectList[i]->vPos;
				pObj->m_pTransform->m_vScale = pNode->m_ObjectList[i]->vScale;
				pObj->m_pTransform->m_qRot = pNode->m_ObjectList[i]->qRot;
				pObj->m_pTransform->SetMatrix(NULL,
					&m_pLight->m_pTransform->m_matView,
					&m_pLight->m_pTransform->m_matProj);
				pObj->Render(pd3dContext);
				if (pModel != nullptr)
				{
					pModel->m_pGraphics->m_isShadowRender = false;
				}
			}
		}
		m_pDepthMap->m_pRT->End();
	}
	return true;
}

bool myQuadTree::ShadowRender(ID3D11DeviceContext * pd3dContext)
{
	myDxState::SetRasterizerState(g_pd3dDevice, g_pImmediateContext,
		myDxState::g_RasterizerDesc);
	ApplySS(pd3dContext, myDxState::g_pSSClampLinear, 1);
	ApplySS(pd3dContext, myDxState::g_pSSShadowMap, 2);

	pd3dContext->UpdateSubresource(m_pDepthMap->m_pCBDepthMap.Get(), 0, NULL, &m_pDepthMap->m_cbData, 0, 0);

	m_pMap->m_pTransform->SetMatrix(NULL,
		&g_pMainCamTransform->m_matView,
		&g_pMainCamTransform->m_matProj);

	m_pMap->Update(pd3dContext);
	m_pMap->PreRender(pd3dContext);
	m_pMap->SettingPipeLine(pd3dContext);
	pd3dContext->VSSetConstantBuffers(2, 1, m_pDepthMap->m_pCBDepthMap.GetAddressOf());
	pd3dContext->PSSetShaderResources(6, 1,
		m_pDepthMap->m_pRT->m_pSRV_DSV.GetAddressOf());
	pd3dContext->IASetIndexBuffer(NULL, DXGI_FORMAT_R32_UINT, 0);
	DrawCulling(pd3dContext);

	for (myNode* pNode : m_DrawNodeList)
	{
		for (int i = 0; i < pNode->m_ObjectList.size(); i++)
		{
			if (!pNode->m_ObjectList[i]->isActive) continue;
			if (!pNode->m_ObjectList[i]->isRender) continue;
			myGameObject* pObj = g_ObjMgr.m_ObjectList[pNode->m_ObjectList[i]->iID];

			pObj->m_pTransform->m_vPos = pNode->m_ObjectList[i]->vPos;
			pObj->m_pTransform->m_vScale = pNode->m_ObjectList[i]->vScale;
			pObj->m_pTransform->m_qRot = pNode->m_ObjectList[i]->qRot;
			pObj->m_pTransform->SetMatrix(NULL,
				&g_pMainCamTransform->m_matView,
				&g_pMainCamTransform->m_matProj);
			pObj->PreRender(pd3dContext);
			pd3dContext->VSSetConstantBuffers(2, 1, m_pDepthMap->m_pCBDepthMap.GetAddressOf());
			pd3dContext->PSSetShaderResources(2, 1,
				m_pDepthMap->m_pRT->m_pSRV_DSV.GetAddressOf());
			pObj->PostRender(pd3dContext);
		}
	}
	return true;
}

bool myQuadTree::Draw(ID3D11DeviceContext*	pd3dContext, myNode* pNode)
{
	if (pNode == nullptr) return false;
	if (pNode->m_isLeaf == true)
	{
		pd3dContext->IASetIndexBuffer(pNode->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		pd3dContext->DrawIndexed(pNode->m_IndexList.size(), 0, 0);
		return true;
	}
	for (int i = 0; i < pNode->m_ChildList.size(); i++)
	{
		Draw(pd3dContext, pNode->m_ChildList[i]);
	}
	return true;
}

bool myQuadTree::DrawCulling(ID3D11DeviceContext*	pd3dContext)
{
	for (myNode* pNode : m_DrawNodeList)
	{
		pd3dContext->IASetIndexBuffer(pNode->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		pd3dContext->DrawIndexed(pNode->m_IndexList.size(), 0, 0);

		//CullingVertex(pNode);
		//g_pImmediateContext->IASetIndexBuffer(pNode->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		//g_pImmediateContext->DrawIndexed(pNode->m_dwFaceNum, 0, 0);
	}
	return true;
}

bool myQuadTree::DrawObject(ID3D11DeviceContext * pd3dContext)
{
	for (myNode* pNode : m_DrawNodeList)
	{
		for (int i = 0; i < pNode->m_ObjectList.size(); i++)
		{
			if (!pNode->m_ObjectList[i]->isActive) continue;
			if (!pNode->m_ObjectList[i]->isRender) continue;
			myGameObject* pObj = g_ObjMgr.m_ObjectList[pNode->m_ObjectList[i]->iID];

			pObj->m_pTransform->m_vPos = pNode->m_ObjectList[i]->vPos;
			pObj->m_pTransform->m_vScale = pNode->m_ObjectList[i]->vScale;
			pObj->m_pTransform->m_qRot = pNode->m_ObjectList[i]->qRot;
			pObj->m_pTransform->SetMatrix(NULL,
				&g_pMainCamTransform->m_matView,
				&g_pMainCamTransform->m_matProj);
			pObj->Render(pd3dContext);
		}
	}
	return true;
}

bool myQuadTree::CullingObject()
{
	m_DrawNodeList.clear();
	for (myNode* pNode : m_LeafNodeList)
	{
		//for (int i = 0; i < pNode->m_ObjectList.size(); i++)
		//{
		//	if (!pNode->m_ObjectList[i]->isActive) continue;
		//	pNode->m_ObjectList[i]->isRender = false;
		//	for (int iVertex = 0; iVertex < 6; iVertex++)
		//	{
		//		if (m_pCullingCamera->m_Frustum.ClassifyPoint(pNode->m_myBox.vPos[iVertex]))
		//		{
		//			pNode->m_ObjectList[i]->isRender = true;
		//
		//			break;
		//		}
		//	}
		//}

		for (int i = 0; i < pNode->m_ObjectList.size(); i++)
		{
			if (!pNode->m_ObjectList[i]->isActive) continue;
			pNode->m_ObjectList[i]->isRender = false;
		}
		for (int i = 0; i < 6; i++)
		{
			if (m_pCullingCamera->m_Frustum.ClassifyPoint(pNode->m_myBox.vPos[i]))
			{
				for (int i = 0; i < pNode->m_ObjectList.size(); i++)
				{
					if (!pNode->m_ObjectList[i]->isActive) continue;
					pNode->m_ObjectList[i]->isRender = true;
				}
				m_DrawNodeList.push_back(pNode);
				break;
			}
		}
	}
	return true;
}

bool myQuadTree::CullingVertex(ID3D11DeviceContext*	pd3dContext, myNode * pNode)
{
	if (pNode == nullptr) return false;
	vector<DWORD> drawIndexList;
	drawIndexList.resize(pNode->m_IndexList.size());
	pNode->m_dwFaceNum = 0;
	for (int i = 0; i < pNode->m_IndexList.size() / 3; i++)
	{
		DWORD index[3];
		index[0] = pNode->m_IndexList[i * 3 + 0];
		index[1] = pNode->m_IndexList[i * 3 + 1];
		index[2] = pNode->m_IndexList[i * 3 + 2];
		Vector3 v[3];
		v[0] = m_pMap->m_VertexList[index[0]].p;
		v[1] = m_pMap->m_VertexList[index[1]].p;
		v[2] = m_pMap->m_VertexList[index[2]].p;
		for (int j = 0; j < 3; j++)
		{
			bool isDraw = m_pCullingCamera->m_Frustum.ClassifyPoint(v[j]);
			if (isDraw)
			{
				drawIndexList[pNode->m_dwFaceNum * 3 + 0] = index[0];
				drawIndexList[pNode->m_dwFaceNum * 3 + 1] = index[1];
				drawIndexList[pNode->m_dwFaceNum * 3 + 2] = index[2];
				pNode->m_dwFaceNum++;
				break;
			}
		}
	}
	pd3dContext->UpdateSubresource(pNode->m_pIndexBuffer.Get()
		, 0, NULL, &drawIndexList.at(0), 0, 0);
	return true;
}

void myQuadTree::RepreshBindingObj(myNode * pNode)
{
	pNode->m_myBox.vMax = { -10000.0f,-10000.0f ,-10000.0f };
	pNode->m_myBox.vMin = { 10000.0f,10000.0f ,10000.0f };
	//임시
	for (DWORD dwIndex = 0; dwIndex < pNode->m_IndexList.size(); dwIndex++)
	{
		Vector3 v = m_pMap->m_VertexList[pNode->m_IndexList[dwIndex]].p;
		if (v.x > pNode->m_myBox.vMax.x)
		{
			pNode->m_myBox.vMax.x = v.x;
		}
		if (v.y > pNode->m_myBox.vMax.y)
		{
			pNode->m_myBox.vMax.y = v.y;
		}
		if (v.z > pNode->m_myBox.vMax.z)
		{
			pNode->m_myBox.vMax.z = v.z;
		}

		if (v.x < pNode->m_myBox.vMin.x)
		{
			pNode->m_myBox.vMin.x = v.x;
		}
		if (v.y < pNode->m_myBox.vMin.y)
		{
			pNode->m_myBox.vMin.y = v.y;
		}
		if (v.z < pNode->m_myBox.vMin.z)
		{
			pNode->m_myBox.vMin.z = v.z;
		}
	}
	pNode->m_myBox.vCenter = (pNode->m_myBox.vMax + pNode->m_myBox.vMin) / 2;

	Vector3 range = pNode->m_myBox.vMax - pNode->m_myBox.vCenter;
	pNode->m_mySphere.vCenter = pNode->m_myBox.vCenter;
	pNode->m_mySphere.fRadius = range.Length();
	pNode->m_myBox.fExtent[0] = range.x;
	pNode->m_myBox.fExtent[1] = range.y;
	pNode->m_myBox.fExtent[2] = range.z;
	pNode->m_myBox.vPos[0] = Vector3(pNode->m_myBox.vMin.x, pNode->m_myBox.vMax.y, pNode->m_myBox.vMin.z);
	pNode->m_myBox.vPos[1] = Vector3(pNode->m_myBox.vMax.x, pNode->m_myBox.vMax.y, pNode->m_myBox.vMin.z);
	pNode->m_myBox.vPos[2] = Vector3(pNode->m_myBox.vMin.x, pNode->m_myBox.vMin.y, pNode->m_myBox.vMin.z);
	pNode->m_myBox.vPos[3] = Vector3(pNode->m_myBox.vMax.x, pNode->m_myBox.vMin.y, pNode->m_myBox.vMin.z);
	pNode->m_myBox.vPos[4] = Vector3(pNode->m_myBox.vMin.x, pNode->m_myBox.vMax.y, pNode->m_myBox.vMax.z);
	pNode->m_myBox.vPos[5] = Vector3(pNode->m_myBox.vMax.x, pNode->m_myBox.vMax.y, pNode->m_myBox.vMax.z);
	pNode->m_myBox.vPos[6] = Vector3(pNode->m_myBox.vMin.x, pNode->m_myBox.vMin.y, pNode->m_myBox.vMax.z);
	pNode->m_myBox.vPos[7] = Vector3(pNode->m_myBox.vMax.x, pNode->m_myBox.vMin.y, pNode->m_myBox.vMax.z);

}

myNode * myQuadTree::CreateNode(myNode * pParentNode, DWORD LeftTop, DWORD RightTop, DWORD LeftBottom, DWORD RightBottom)
{
	if (m_pMap == nullptr) return nullptr;
	myNode* newNode = new myNode;
	if (pParentNode != nullptr)
	{
		newNode->m_pParent = pParentNode;
		newNode->m_iDepth = pParentNode->m_iDepth + 1;
	}
	newNode->m_CornerIndexList.resize(4);
	newNode->m_CornerIndexList[0] = LeftTop;
	newNode->m_CornerIndexList[1] = RightTop;
	newNode->m_CornerIndexList[2] = LeftBottom;
	newNode->m_CornerIndexList[3] = RightBottom;

	newNode->m_CornerList.resize(4);
	newNode->m_CornerList[0] = m_pMap->m_VertexList[LeftTop];
	newNode->m_CornerList[1] = m_pMap->m_VertexList[RightTop];
	newNode->m_CornerList[2] = m_pMap->m_VertexList[LeftBottom];
	newNode->m_CornerList[3] = m_pMap->m_VertexList[RightBottom];

	DWORD dwNodeWidth = RightTop - LeftTop;
	DWORD dwNumCols = RightTop - LeftTop + 1;
	DWORD indexSize = dwNodeWidth * dwNodeWidth * 2 * 3;
	DWORD vertexSize = dwNumCols * dwNumCols;
	newNode->m_VertexList.resize(vertexSize);
	newNode->m_IndexList.resize(indexSize);
	newNode->m_NodeIndexList.resize(indexSize);
	//셀기준
	DWORD indexWidth = m_pMap->m_iNumCols;
	DWORD indexStartRow = LeftTop / indexWidth;
	DWORD indexEndRow = LeftBottom / indexWidth;
	DWORD indexStartCol = LeftTop % indexWidth;
	DWORD indexEndCol = RightTop % indexWidth;

	int index = 0;
	int nodeIndex = 0;
	int vertex = 0;
	//+1 인 이유는 점기준이라
	for (DWORD Row = indexStartRow; Row < indexEndRow + 1; Row++)
	{
		for (DWORD Col = indexStartCol; Col < indexEndCol + 1; Col++)
		{
			newNode->m_VertexList[vertex++] = m_pMap->m_VertexList[Row * indexWidth + Col];
		}
	}
	//각각 가지고있는 버텍스 기준 인덱스 저장
	for (DWORD Row = 0; Row < dwNodeWidth; Row++)
	{
		for (DWORD Col = 0; Col < dwNodeWidth; Col++)
		{
			newNode->m_NodeIndexList[nodeIndex++] = Row * dwNumCols + Col;
			newNode->m_NodeIndexList[nodeIndex++] = Row * dwNumCols + (Col + 1);
			newNode->m_NodeIndexList[nodeIndex++] = (Row + 1) * dwNumCols + Col;
			newNode->m_NodeIndexList[nodeIndex++] = (Row + 1) * dwNumCols + Col;
			newNode->m_NodeIndexList[nodeIndex++] = Row * dwNumCols + (Col + 1);
			newNode->m_NodeIndexList[nodeIndex++] = (Row + 1) * dwNumCols + (Col + 1);
		}
	}
	//맵 버텍스 기준 인덱스 저장
	for (DWORD Row = indexStartRow; Row < indexEndRow; Row++)
	{
		for (DWORD Col = indexStartCol; Col < indexEndCol; Col++)
		{

			//int a = Row * indexEndCol + Col;
			//0 1 2 2 1 3
			newNode->m_IndexList[index++] = Row * indexWidth + Col;
			newNode->m_IndexList[index++] = Row * indexWidth + (Col + 1);
			newNode->m_IndexList[index++] = (Row + 1) * indexWidth + Col;
			newNode->m_IndexList[index++] = (Row + 1) * indexWidth + Col;
			newNode->m_IndexList[index++] = Row * indexWidth + (Col + 1);
			newNode->m_IndexList[index++] = (Row + 1) * indexWidth + (Col + 1);
		}
	}

	newNode->m_myBox.vMax = { -10000.0f,-10000.0f ,-10000.0f };
	newNode->m_myBox.vMin = { 10000.0f,10000.0f ,10000.0f };
	//임시
	for (DWORD dwIndex = 0; dwIndex < newNode->m_IndexList.size(); dwIndex++)
	{
		Vector3 v = m_pMap->m_VertexList[newNode->m_IndexList[dwIndex]].p;
		if (v.x > newNode->m_myBox.vMax.x)
		{
			newNode->m_myBox.vMax.x = v.x;
		}
		if (v.y > newNode->m_myBox.vMax.y)
		{
			newNode->m_myBox.vMax.y = v.y;
		}
		if (v.z > newNode->m_myBox.vMax.z)
		{
			newNode->m_myBox.vMax.z = v.z;
		}

		if (v.x < newNode->m_myBox.vMin.x)
		{
			newNode->m_myBox.vMin.x = v.x;
		}
		if (v.y < newNode->m_myBox.vMin.y)
		{
			newNode->m_myBox.vMin.y = v.y;
		}
		if (v.z < newNode->m_myBox.vMin.z)
		{
			newNode->m_myBox.vMin.z = v.z;
		}
	}
	newNode->m_myBox.vCenter = (newNode->m_myBox.vMax + newNode->m_myBox.vMin) / 2;

	Vector3 range = newNode->m_myBox.vMax - newNode->m_myBox.vCenter;
	newNode->m_mySphere.vCenter = newNode->m_myBox.vCenter;
	newNode->m_mySphere.fRadius = range.Length();
	newNode->m_myBox.vAxis[0] = Vector3(1.0f, 0.0f, 0.0f);
	newNode->m_myBox.vAxis[1] = Vector3(0.0f, 1.0f, 0.0f);
	newNode->m_myBox.vAxis[2] = Vector3(0.0f, 0.0f, 1.0f);
	newNode->m_myBox.fExtent[0] = range.x;
	newNode->m_myBox.fExtent[1] = range.y;
	newNode->m_myBox.fExtent[2] = range.z;
	newNode->m_myBox.vPos[0] = Vector3(newNode->m_myBox.vMin.x, newNode->m_myBox.vMax.y, newNode->m_myBox.vMin.z);
	newNode->m_myBox.vPos[1] = Vector3(newNode->m_myBox.vMax.x, newNode->m_myBox.vMax.y, newNode->m_myBox.vMin.z);
	newNode->m_myBox.vPos[2] = Vector3(newNode->m_myBox.vMin.x, newNode->m_myBox.vMin.y, newNode->m_myBox.vMin.z);
	newNode->m_myBox.vPos[3] = Vector3(newNode->m_myBox.vMax.x, newNode->m_myBox.vMin.y, newNode->m_myBox.vMin.z);
	newNode->m_myBox.vPos[4] = Vector3(newNode->m_myBox.vMin.x, newNode->m_myBox.vMax.y, newNode->m_myBox.vMax.z);
	newNode->m_myBox.vPos[5] = Vector3(newNode->m_myBox.vMax.x, newNode->m_myBox.vMax.y, newNode->m_myBox.vMax.z);
	newNode->m_myBox.vPos[6] = Vector3(newNode->m_myBox.vMin.x, newNode->m_myBox.vMin.y, newNode->m_myBox.vMax.z);
	newNode->m_myBox.vPos[7] = Vector3(newNode->m_myBox.vMax.x, newNode->m_myBox.vMin.y, newNode->m_myBox.vMax.z);

	newNode->m_pIndexBuffer.Attach(StaticGraphics::CreateIndexBuffer(
		g_pd3dDevice, &newNode->m_IndexList.at(0),
		newNode->m_IndexList.size(),
		sizeof(DWORD)));
	return newNode;
}

bool myQuadTree::DrawNodeCollider(ID3D11DeviceContext*	pd3dContext, myNode* pNode)
{
	m_pQuadTreeLine->Draw(pd3dContext, pNode->m_myBox.vPos[0], pNode->m_myBox.vPos[1], Vector4(1,1,1,1));
	m_pQuadTreeLine->Draw(pd3dContext, pNode->m_myBox.vPos[2], pNode->m_myBox.vPos[3], Vector4(1,1,1,1));
	m_pQuadTreeLine->Draw(pd3dContext, pNode->m_myBox.vPos[0], pNode->m_myBox.vPos[2], Vector4(1,1,1,1));
	m_pQuadTreeLine->Draw(pd3dContext, pNode->m_myBox.vPos[1], pNode->m_myBox.vPos[3], Vector4(1,1,1,1));
	m_pQuadTreeLine->Draw(pd3dContext, pNode->m_myBox.vPos[4], pNode->m_myBox.vPos[5], Vector4(1,1,1,1));
	m_pQuadTreeLine->Draw(pd3dContext, pNode->m_myBox.vPos[6], pNode->m_myBox.vPos[7], Vector4(1,1,1,1));
	m_pQuadTreeLine->Draw(pd3dContext, pNode->m_myBox.vPos[4], pNode->m_myBox.vPos[6], Vector4(1,1,1,1));
	m_pQuadTreeLine->Draw(pd3dContext, pNode->m_myBox.vPos[5], pNode->m_myBox.vPos[7], Vector4(1,1,1,1));
	m_pQuadTreeLine->Draw(pd3dContext, pNode->m_myBox.vPos[0], pNode->m_myBox.vPos[4], Vector4(1,1,1,1));
	m_pQuadTreeLine->Draw(pd3dContext, pNode->m_myBox.vPos[1], pNode->m_myBox.vPos[5], Vector4(1,1,1,1));
	m_pQuadTreeLine->Draw(pd3dContext, pNode->m_myBox.vPos[2], pNode->m_myBox.vPos[6], Vector4(1,1,1,1));
	m_pQuadTreeLine->Draw(pd3dContext, pNode->m_myBox.vPos[3], pNode->m_myBox.vPos[7], Vector4(1,1,1,1));
	return true;
}

bool myQuadTree::DrawCollider(ID3D11DeviceContext * pd3dContext)
{
	m_pQuadTreeLine->m_pTransform->SetMatrix(NULL,
		&g_pMainCamTransform->m_matView,
		&g_pMainCamTransform->m_matProj);
	for (myNode* pNode : m_DrawNodeList)
	{
		DrawNodeCollider(pd3dContext, pNode);
	}
	return true;
}

//bool myQuadTree::AddObject(SampleIns * ins)
//{
//	return false;
//}
//
//bool myQuadTree::RepreshQuadTreeObject()
//{
//	return false;
//}


myQuadTree::myQuadTree(void)
{
	m_pRootNode = NULL;
	m_iMaxdepth = 4; // 쿼드트리 뎁스 1 = 안쪼갬 2 = 2번쪼갬
}

myQuadTree::~myQuadTree(void)
{
	Release();
	if (m_pDepthMap) m_pDepthMap->Release();
	SAFE_DEL(m_pDepthMap);
}
