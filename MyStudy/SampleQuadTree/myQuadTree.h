#pragma once
#include "myNode.h"
#include "myMap.h"
class myQuadTree
{
public:
	myMap*			m_pMap;
	//�ֻ��� ���
	myNode*			m_pRootNode;
	//�ִ� ���� �Ѱ�
	int				m_iMaxdepth;
	//�ּ� ũ��
public:
	bool			CreateQuadTree(myMap* pMap);
	//����
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

