#include "myQuadTree.h"

bool myQuadTree::CreateQuadTree(myMap* pMap)
{
	m_pMap = pMap;
	m_pRootNode = CreateNode(nullptr, 0,
		m_pMap->m_iNumCols - 1,
		m_pMap->m_iNumVertices - 1 - (m_pMap->m_iNumCols - 1),
		m_pMap->m_iNumVertices - 1);

	Partition(m_pRootNode);
	return true;
}

bool myQuadTree::Partition(myNode* pParentNode)
{
	if (pParentNode->m_iDepth + 1 > m_iMaxdepth)
	{
		pParentNode->m_isLeaf = true;
		return false;
	}
	//크기제한

	DWORD CT, CL, CC, CR, CB;
	CT = (pParentNode->m_IndexNumList[0] + pParentNode->m_IndexNumList[1]) / 2;
	CL = (pParentNode->m_IndexNumList[0] + pParentNode->m_IndexNumList[2]) / 2;
	CR = (pParentNode->m_IndexNumList[1] + pParentNode->m_IndexNumList[3]) / 2;
	CB = (pParentNode->m_IndexNumList[2] + pParentNode->m_IndexNumList[3]) / 2;
	CC = (CT + CB) / 2;
	pParentNode->m_ChildList.resize(4);
	pParentNode->m_ChildList[0] = CreateNode(pParentNode, pParentNode->m_IndexNumList[0], CT, CL, CC);
	pParentNode->m_ChildList[1] = CreateNode(pParentNode, CT, pParentNode->m_IndexNumList[1], CC, CR);
	pParentNode->m_ChildList[2] = CreateNode(pParentNode, CL, CC, pParentNode->m_IndexNumList[2], CB);
	pParentNode->m_ChildList[3] = CreateNode(pParentNode, CC, CR, CB, pParentNode->m_IndexNumList[3]);

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
	return false;
}

bool myQuadTree::Render()
{
	m_pMap->Update();
	m_pMap->SettingPipeLine();
	g_pImmediateContext->IASetIndexBuffer(NULL, DXGI_FORMAT_R32_UINT, 0);
	DrawCulling();
	//Draw(m_pRootNode);
	return true;
}

bool myQuadTree::Draw(myNode* pNode)
{
	if (pNode == nullptr) return false;
	if (pNode->m_isLeaf == true)
	{
		g_pImmediateContext->IASetIndexBuffer(pNode->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		g_pImmediateContext->DrawIndexed(pNode->m_IndexList.size(), 0, 0);
		return true;
	}
	for (int i = 0; i < pNode->m_ChildList.size(); i++)
	{
		Draw(pNode->m_ChildList[i]);
	}
	return true;
}

bool myQuadTree::DrawCulling()
{
	m_DrawNodeList.clear();
	FrustumCulling(m_pRootNode);
	for (myNode* pNode : m_DrawNodeList)
	{
		g_pImmediateContext->IASetIndexBuffer(pNode->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		g_pImmediateContext->DrawIndexed(pNode->m_IndexList.size(), 0, 0);
	}
	return true;
}

bool myQuadTree::FrustumCulling(myNode * pNode)
{
	if (pNode == nullptr) return false;
	if (pNode->m_isLeaf == true)
	{
		for (int i = 0; i < pNode->m_CornerList.size(); i++)
		{
			if (g_CamMgr.m_pMainCamera->m_Frustum.ClassifyPoint(pNode->m_CornerList[i]))
			{
				m_DrawNodeList.push_back(pNode);
				break;
			}
		}
	}
	for (int i = 0; i < pNode->m_ChildList.size(); i++)
	{
		FrustumCulling(pNode->m_ChildList[i]);
	}
	return true;
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
	newNode->m_IndexNumList.resize(4);
	newNode->m_IndexNumList[0] = LeftTop;
	newNode->m_IndexNumList[1] = RightTop;
	newNode->m_IndexNumList[2] = LeftBottom;
	newNode->m_IndexNumList[3] = RightBottom;

	newNode->m_CornerList.resize(4);
	newNode->m_CornerList[0] = m_pMap->m_VertexList[LeftTop].p;
	newNode->m_CornerList[1] = m_pMap->m_VertexList[RightTop].p;
	newNode->m_CornerList[2] = m_pMap->m_VertexList[LeftBottom].p;
	newNode->m_CornerList[3] = m_pMap->m_VertexList[RightBottom].p;

	newNode->m_myBox.vMax = newNode->m_CornerList[1];
	newNode->m_myBox.vMin = newNode->m_CornerList[2];
	newNode->m_myBox.vCenter = (newNode->m_myBox.vMax + newNode->m_myBox.vMin) / 2;

	newNode->m_myBox.vPos[0] = newNode->m_myBox.vPos[4] = newNode->m_CornerList[0];
	newNode->m_myBox.vPos[1] = newNode->m_myBox.vPos[5] = newNode->m_CornerList[1];
	newNode->m_myBox.vPos[2] = newNode->m_myBox.vPos[6] = newNode->m_CornerList[2];
	newNode->m_myBox.vPos[3] = newNode->m_myBox.vPos[7] = newNode->m_CornerList[3];

	newNode->m_IndexList.resize(6);
	newNode->m_IndexList[0] = newNode->m_IndexNumList[0];
	newNode->m_IndexList[1] = newNode->m_IndexNumList[1];
	newNode->m_IndexList[2] = newNode->m_IndexNumList[2];
	newNode->m_IndexList[3] = newNode->m_IndexNumList[2];
	newNode->m_IndexList[4] = newNode->m_IndexNumList[1];
	newNode->m_IndexList[5] = newNode->m_IndexNumList[3];

	newNode->m_pIndexBuffer.Attach(CreateIndexBuffer(
		g_pd3dDevice, &newNode->m_IndexList.at(0),
		newNode->m_IndexList.size(),
		sizeof(DWORD)));
	return newNode;
}


myQuadTree::myQuadTree(void)
{
	m_pRootNode = NULL;
	m_iMaxdepth = 4;
}

myQuadTree::~myQuadTree(void)
{
	Release();
}
