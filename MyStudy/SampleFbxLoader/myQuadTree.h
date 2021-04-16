#pragma once
#include "myNode.h"
#include "myMap.h"
#include "myCameraManager.h"
class myQuadTree
{
public:
	myMap*			m_pMap;
	//최상위 노드
	myNode*			m_pRootNode;
	//최대 분할 한계
	int				m_iMaxdepth;
	//최소 크기

	//뿌려줄 노드
	vector<myNode*> m_LeafNodeList;
	vector<myNode*>	m_DrawNodeList;
public:
	bool			CreateQuadTree(myMap* pMap);
	//분할
	bool			Partition(myNode* pParentNode);
	virtual bool	Release();
	virtual bool	PreFrame();
	virtual bool	Frame();
	virtual bool	Render(ID3D11DeviceContext*	pd3dContext);
	virtual bool	Draw(ID3D11DeviceContext*	pd3dContext, myNode* pNode);
	virtual bool	DrawCulling(ID3D11DeviceContext*	pd3dContext);
	virtual bool	CullingNode();
	virtual bool	CullingVertex(ID3D11DeviceContext*	pd3dContext, myNode* pNode);

	myNode*			CreateNode(myNode* pParentNode, DWORD LeftTop, DWORD RightTop, DWORD LeftBottom, DWORD RightBottom);
public:
	myQuadTree();
	virtual ~myQuadTree();
};

