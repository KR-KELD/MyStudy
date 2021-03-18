#pragma once
#include "myNode.h"
#include "myMap.h"
#include "myCameraManager.h"
class myQuadTree
{
	typedef vector<myNode*> NodeList;

public:
	myMap*				m_pMap;
	//�ֻ��� ���
	myNode*				m_pRootNode;
	//�ִ� ���� �Ѱ�
	int					m_iMaxdepth;
	//�ּ� ũ��

	//���ϴ� ��� ����Ʈ
	vector<myNode*>		m_LeafNodeList;
	//�׷��� ��� ����Ʈ
	vector<myNode*>		m_DrawNodeList;
	//���̺��� ������ ��� ����Ʈ
	vector<NodeList>	m_DepthNodeList;
public:
	void			SetNeighborNode(myNode* pNode);
	bool			CreateQuadTree(myMap* pMap);
	//����
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

