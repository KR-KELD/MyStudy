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

bool myQuadTree::Render(ID3D11DeviceContext*	pd3dContext)
{
	m_pMap->Update(g_pImmediateContext);
	m_pMap->PreRender(pd3dContext);
	m_pMap->SettingPipeLine(g_pImmediateContext);
	pd3dContext->IASetIndexBuffer(NULL, DXGI_FORMAT_R32_UINT, 0);
	DrawCulling(pd3dContext);
	//Draw(m_pRootNode);
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
	CullingNode();
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

bool myQuadTree::CullingNode()
{
	m_DrawNodeList.clear();
	for (myNode* pNode : m_LeafNodeList)
	{
		for (int i = 0; i < 6; i++)
		{
			if (g_CamMgr.m_pMainCamera->m_Frustum.ClassifyPoint(pNode->m_myBox.vPos[i]))
			{
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
	pd3dContext->UpdateSubresource(pNode->m_pIndexBuffer.Get()
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
			//버텍스리스트를 0부터 담으면 문제
			//중간부터 담아도 메모리상 문제 질문

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

	newNode->m_pIndexBuffer.Attach(StaticGraphics::CreateIndexBuffer(
		g_pd3dDevice, &newNode->m_IndexList.at(0),
		newNode->m_IndexList.size(),
		sizeof(DWORD)));
	return newNode;
}


myQuadTree::myQuadTree(void)
{
	m_pRootNode = NULL;
	m_iMaxdepth = 3; // 쿼드트리 뎁스 1 = 안쪼갬 2 = 2번쪼갬
}

myQuadTree::~myQuadTree(void)
{
	Release();
}
