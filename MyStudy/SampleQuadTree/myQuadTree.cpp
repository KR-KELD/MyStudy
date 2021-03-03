#include "myQuadTree.h"

bool myQuadTree::Build(float fWidth, float fHeight)
{

	return true;
}

bool myQuadTree::BuildTree(myNode * pNode)
{
	return false;
}

bool myQuadTree::SubDivide(myNode * pNode)
{
	return false;
}

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

myNode * myQuadTree::CreateNode(myNode * pParentNode, float fTopLeft, float fTopRight, float fBottomLeft, float fBottomRight)
{
	return nullptr;
}


myQuadTree::myQuadTree(void)
{
	m_iRenderDepth = 0;
	m_iMaxDepth = 0;
	m_iMaxDepthLimit = 1;
	m_fMinDivideSize = 2;
	m_pRootNode = 0;
}

myQuadTree::~myQuadTree(void)
{
	Release();
}
