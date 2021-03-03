#pragma once
#include "myNode.h"
#include "myCameraManager.h"
#include <queue>

class myQuadTree
{
public:
	//�ֻ��� ���
	myNode*						m_pRootNode;
	//�ӽ� ť
	queue<myNode*>				m_QuadTreeQueue;
	//�׷��� ������Ʈ ����Ʈ
	vector<myGameObject*>		m_DrawObjList;
	//�׷��� ��� ����Ʈ
	vector< myNode* >			m_DrawNodeList;
	int				m_iRenderDepth;
	int				m_iMaxDepthLimit;
	int				m_iMaxDepth;
	float			m_fWidth;
	float			m_fHeight;
	float			m_fMinDivideSize; 
public:
	bool			Build(float fWidth, float fHeight);
	virtual bool	BuildTree(myNode* pNode);
	virtual bool	SubDivide(myNode* pNode);
	virtual bool	Release();
	virtual bool	PreFrame();
	virtual bool	Frame();
	myNode*			CreateNode(myNode* pParentNode, float fTopLeft, float fTopRight, float fBottomLeft, float fBottomRight);
public:
	myQuadTree();
	virtual ~myQuadTree();
};

