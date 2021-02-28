#pragma once
#include "myNode.h"
#include "myCameraManager.h"
#include <queue>

class myQuadTree
{
public:
	enum QUADTREECORNER { tTL = 0, tTR = 1, tBL = 2, tBR = 3, };
	int				m_iRenderDepth;
	void			SetRenderDepth(DWORD dwDepth);
	myNode*					m_pRootNode;
	queue<myNode*>			m_QuadTreeQueue;
	vector<myBaseObj*>		m_DrawObjList;
	vector< myNode* >		m_DrawNodeList;
	bool			m_bUsedIndexList;		// ���� �ε��� ����Ͽ� ��������	
	int				m_iMaxDepthLimit;		// ��� ������ �ִ� �ѵ�
	void			SetMaxDepthLimit(int iMaxDepth) { m_iMaxDepthLimit = iMaxDepth; }
	int				m_iMaxDepth;			// �� ���� ����� ����( ���� ) ����
	float			m_fWidth;
	float			m_fHeight;
	// �ּ�	���� ũ��
	float			m_fMinDivideSize;
	void			SetMinDivideSize(int iMinDivideSize) { m_fMinDivideSize = (float)iMinDivideSize; }
public:
	bool			Build(float fWidth, float fHeight);
	virtual bool	BuildTree(myNode* pNode);
	virtual bool	SubDivide(myNode* pNode);
	virtual bool	Release();
	virtual bool	PreFrame();
	virtual bool	Frame();
	myNode*			CreateNode(myNode* pParentNode, float fTopLeft, float fTopRight, float fBottomLeft, float fBottomRight);

	int				AddObject(myBaseObj* pObj);
	myNode*			FindNode(myNode* pNode, myBaseObj* pObj);
	int				CheckRect(myNode* pNode, myBaseObj* pObj);

	void			VisibleNode(myNode* pNode);
	void			VisibleObject(myNode* pNode);
	virtual void	DrawFindNode(myNode* pNode);

	DWORD			CheckSize(DWORD dwSize);
	void			SetNeighborNode(myNode* pNode);
	void			FindNeighborNode(myNode* pNode);
public:
	myQuadTree();
	virtual ~myQuadTree();
};

