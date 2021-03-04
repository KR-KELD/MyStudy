#pragma once
#include "myNode.h"
#include "myMap.h"
class myQuadTree
{
public:
	myMap*			m_pMap;
	//최상위 노드
	myNode*			m_pRootNode;
	//최대 분할 한계
	int				m_iMaxdepth;
	//최소 크기
public:
	bool			CreateQuadTree(myMap* pMap);
	//분할
	bool			Partition(myNode* pParentNode);
	virtual bool	Release();
	virtual bool	PreFrame();
	virtual bool	Frame();
	virtual bool	Render();
	virtual bool	Draw(myNode* pNode);

	myNode*			CreateNode(myNode* pParentNode, DWORD LeftTop, DWORD RightTop, DWORD LeftBottom, DWORD RightBottom);
public:
	myQuadTree();
	virtual ~myQuadTree();
};

