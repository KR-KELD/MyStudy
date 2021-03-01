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
	//최상위 노드
	myNode*					m_pRootNode;
	//임시 큐
	queue<myNode*>			m_QuadTreeQueue;
	//그려줄 오브젝트 리스트
	vector<myBaseObj*>		m_DrawObjList;
	//그려줄 노드 리스트
	vector< myNode* >		m_DrawNodeList;
	bool			m_bUsedIndexList;		// 정점 인덱스 사용하여 공간분할	
	int				m_iMaxDepthLimit;		// 노드 분할의 최대 한도
	void			SetMaxDepthLimit(int iMaxDepth) { m_iMaxDepthLimit = iMaxDepth; }
	int				m_iMaxDepth;			// 현 리프 노드의 레벨( 깊이 ) 개수
	//쿼드트리의 가로 크기
	float			m_fWidth;
	//쿼드트리의 세로크기
	float			m_fHeight;
	// 최소	분할 크기
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

