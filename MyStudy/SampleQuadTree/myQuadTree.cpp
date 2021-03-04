#include "myQuadTree.h"

bool myQuadTree::CreateQuadTree(myMap* pMap)
{
	m_pMap = pMap;
	m_pRootNode = CreateNode(nullptr, 0,
		m_pMap->m_iNumCols - 1,
		m_pMap->m_iNumVertices - (m_pMap->m_iNumCols - 1),
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
	Draw(m_pRootNode);
	return true;
}

bool myQuadTree::Draw(myNode* pNode)
{
	if (pNode == nullptr) return false;
	if (pNode->m_isLeaf == true)
	{
		m_pMap->Update();
		m_pMap->PreRender();
		UINT iStride = sizeof(PNCT_VERTEX);
		UINT iOffset = 0;
		g_pImmediateContext->IASetVertexBuffers(0, 1, m_pMap->m_pVertexBuffer.GetAddressOf(), &iStride, &iOffset);
		g_pImmediateContext->IASetIndexBuffer(pNode->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		g_pImmediateContext->IASetInputLayout(m_pMap->m_pInputLayout.Get());
		g_pImmediateContext->VSSetConstantBuffers(0, 1, m_pMap->m_pConstantBuffer.GetAddressOf());
		g_pImmediateContext->PSSetConstantBuffers(0, 1, m_pMap->m_pConstantBuffer.GetAddressOf());
		g_pImmediateContext->VSSetShader(m_pMap->m_pVertexShader.Get(), NULL, 0);
		g_pImmediateContext->PSSetShader(m_pMap->m_pPixelShader.Get(), NULL, 0);
		g_pImmediateContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)m_pMap->m_iTopology);
		if (m_pMap->m_pTexture != nullptr)
		{
			g_pImmediateContext->PSSetShaderResources(0, 1,
				m_pMap->m_pTexture->m_pTextureSRV.GetAddressOf());
		}
		g_pImmediateContext->DrawIndexed(pNode->m_IndexList.size(), 0, 0);
		return true;
	}
	Draw(pNode->m_ChildList[0]);
	Draw(pNode->m_ChildList[1]);
	Draw(pNode->m_ChildList[2]);
	//Draw(pNode->m_ChildList[3]);
	return true;
}

myNode * myQuadTree::CreateNode(myNode * pParentNode, DWORD LeftTop, DWORD RightTop, DWORD LeftBottom, DWORD RightBottom)
{
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
	m_iMaxdepth = 3;
}

myQuadTree::~myQuadTree(void)
{
	Release();
}
