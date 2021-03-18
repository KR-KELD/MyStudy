#pragma once
#include "myNode.h"
#include "myMap.h"
#include "myCameraManager.h"
class myQuadTree
{
	typedef vector<myNode*> NodeList;

public:
	myMap*				m_pMap;
	//최상위 노드
	myNode*				m_pRootNode;
	//최대 분할 한계
	int					m_iMaxdepth;
	//최소 크기

	//최하단 노드 리스트
	vector<myNode*>		m_LeafNodeList;
	//그려줄 노드 리스트
	vector<myNode*>		m_DrawNodeList;
	//깊이별로 나눠둔 노드 리스트
	vector<NodeList>	m_DepthNodeList;
public:
	void			SetNeighborNode(myNode* pNode);
	bool			CreateQuadTree(myMap* pMap);
	//분할
	bool			Partition(myNode* pParentNode);
	virtual bool	Release();
	virtual bool	PreFrame();
	virtual bool	Frame();
	virtual bool	Render();
	virtual bool	Draw(myNode* pNode);
	virtual bool	DrawCulling();
	virtual bool	CullingNode();
	virtual bool	CullingVertex(myNode* pNode);

	myNode*			CreateNode(myNode* pParentNode, DWORD LeftTop, DWORD RightTop, DWORD LeftBottom, DWORD RightBottom);
public:
	myQuadTree();
	virtual ~myQuadTree();
};

