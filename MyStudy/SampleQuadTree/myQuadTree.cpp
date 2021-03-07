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
	if (pParentNode->m_CornerIndexList[1] - pParentNode->m_CornerIndexList[0] <= 1)
	{
		m_LeafNodeList.push_back(pParentNode);
		pParentNode->m_isLeaf = true;
		return false;
	}
	if (pParentNode->m_iDepth + 1 > m_iMaxdepth)
	{
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
	CullingNode();
	for (myNode* pNode : m_DrawNodeList)
	{
		g_pImmediateContext->IASetIndexBuffer(pNode->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		g_pImmediateContext->DrawIndexed(pNode->m_IndexList.size(), 0, 0);


		//CullingVertex(pNode);
		//g_pImmediateContext->IASetIndexBuffer(pNode->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		//g_pImmediateContext->DrawIndexed(pNode->m_dwFaceNum, 0, 0);
	}
	return true;
}

bool myQuadTree::CullingNode()
{
	m_DrawNodeList.clear();
	for (myNode* pNode : m_LeafNodeList)
	{
		for (int i = 0; i < pNode->m_CornerList.size(); i++)
		{
			if (g_CamMgr.m_pMainCamera->m_Frustum.ClassifyPoint(pNode->m_CornerList[i].p))
			{
				m_DrawNodeList.push_back(pNode);
				break;
			}
		}
	}
	return true;
}

bool myQuadTree::CullingVertex(myNode * pNode)
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
			bool isDraw = g_CamMgr.m_pMainCamera->m_Frustum.ClassifyPoint(v[j]);
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
	g_pImmediateContext->UpdateSubresource(pNode->m_pIndexBuffer.Get()
		, 0, NULL, &drawIndexList.at(0), 0, 0);
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

	newNode->m_myBox.vMax = Vector3(newNode->m_CornerList[1].p.x, 500.0f, newNode->m_CornerList[1].p.z);
	newNode->m_myBox.vMin = Vector3(newNode->m_CornerList[2].p.x, -500.0f, newNode->m_CornerList[2].p.z);
	newNode->m_myBox.vCenter = (newNode->m_myBox.vMax + newNode->m_myBox.vMin) / 2;
	Vector3 range = newNode->m_myBox.vMax - newNode->m_myBox.vCenter;
	newNode->m_myBox.fExtent[0] = range.x;
	newNode->m_myBox.fExtent[1] = range.y;
	newNode->m_myBox.fExtent[2] = range.z;
	newNode->m_myBox.vPos[0] = Vector3(newNode->m_myBox.vMin.x, newNode->m_myBox.vMax.y, newNode->m_myBox.vMin.z);
	newNode->m_myBox.vPos[1] = Vector3(newNode->m_myBox.vMax.x, newNode->m_myBox.vMax.y, newNode->m_myBox.vMin.z);
	newNode->m_myBox.vPos[2] = Vector3(newNode->m_myBox.vMax.x, newNode->m_myBox.vMin.y, newNode->m_myBox.vMin.z);
	newNode->m_myBox.vPos[3] = Vector3(newNode->m_myBox.vMin.x, newNode->m_myBox.vMin.y, newNode->m_myBox.vMin.z);
	newNode->m_myBox.vPos[4] = Vector3(newNode->m_myBox.vMin.x, newNode->m_myBox.vMax.y, newNode->m_myBox.vMax.z);
	newNode->m_myBox.vPos[5] = Vector3(newNode->m_myBox.vMax.x, newNode->m_myBox.vMax.y, newNode->m_myBox.vMax.z);
	newNode->m_myBox.vPos[6] = Vector3(newNode->m_myBox.vMax.x, newNode->m_myBox.vMin.y, newNode->m_myBox.vMax.z);
	newNode->m_myBox.vPos[7] = Vector3(newNode->m_myBox.vMin.x, newNode->m_myBox.vMin.y, newNode->m_myBox.vMax.z);

	DWORD vertexSize = (RightTop - LeftTop) * (RightTop - LeftTop);
	DWORD indexSize = vertexSize * 2 * 3;
	newNode->m_VertexList.resize(vertexSize);
	newNode->m_IndexList.resize(indexSize);

	DWORD indexWidth = m_pMap->m_iNumCols;
	DWORD indexStartRow = LeftTop / indexWidth;
	DWORD indexEndRow = LeftBottom / indexWidth;
	DWORD indexStartCol = LeftTop % indexWidth;
	DWORD indexEndCol = RightTop % indexWidth;

	int index = 0;
	for (DWORD Row = indexStartRow; Row < indexEndRow; Row++)
	{
		for (DWORD Col = indexStartCol; Col < indexEndCol; Col++)
		{
			//0 1 2 2 1 3
			newNode->m_IndexList[index++] = Row * indexWidth + Col;
			newNode->m_IndexList[index++] = Row * indexWidth + (Col + 1);
			newNode->m_IndexList[index++] = (Row + 1) * indexWidth + Col;
			newNode->m_IndexList[index++] = (Row + 1) * indexWidth + Col;
			newNode->m_IndexList[index++] = Row * indexWidth + (Col + 1);
			newNode->m_IndexList[index++] = (Row + 1) * indexWidth + (Col + 1);
		}
	}

	//newNode->m_IndexList.resize(6);
	//newNode->m_IndexList[0] = newNode->m_CornerIndexList[0];
	//newNode->m_IndexList[1] = newNode->m_CornerIndexList[1];
	//newNode->m_IndexList[2] = newNode->m_CornerIndexList[2];
	//newNode->m_IndexList[3] = newNode->m_CornerIndexList[2];
	//newNode->m_IndexList[4] = newNode->m_CornerIndexList[1];
	//newNode->m_IndexList[5] = newNode->m_CornerIndexList[3];

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
