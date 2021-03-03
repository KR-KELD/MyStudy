#include "myQuadTree.h"

bool myQuadTree::Release()
{
	SAFE_DEL(m_pRootNode);
	return true;
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
