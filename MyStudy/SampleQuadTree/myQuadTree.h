#pragma once
#include "myNode.h"
#include "myMap.h"
#include "myCameraManager.h"
class myQuadTree
{
public:
	myMap*			m_pMap;
	//�ֻ��� ���
	myNode*			m_pRootNode;
	//�ִ� ���� �Ѱ�
	int				m_iMaxdepth;
	//�ּ� ũ��

	//�ѷ��� ���
	vector<myNode*>	m_DrawNodeList;
public:
	bool			CreateQuadTree(myMap* pMap);
	//����
	bool			Partition(myNode* pParentNode);
	virtual bool	Release();
	virtual bool	PreFrame();
	virtual bool	Frame();
	virtual bool	Render();
	virtual bool	Draw(myNode* pNode);
	virtual bool	DrawCulling();
	virtual bool	FrustumCulling(myNode* pNode);

	myNode*			CreateNode(myNode* pParentNode, DWORD LeftTop, DWORD RightTop, DWORD LeftBottom, DWORD RightBottom);
public:
	myQuadTree();
	virtual ~myQuadTree();
};

