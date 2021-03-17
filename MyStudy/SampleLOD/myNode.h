#pragma once
#include "myGraphics.h"

class myNode
{
public:
	myNode*					m_pParent;
	//깊이
	int						m_iDepth;
	//최하단 노드인가
	bool					m_isLeaf;
	//노드의 인덱스 가로 값
	int						m_iIndexWidth;
	//노드의 인덱스 세로 값
	int						m_iIndexHeight;
	// 바운딩 박스
	MY_BOX					m_myBox;
	//인접 노드 리스트
	vector<myNode*>			m_NeighborList;
	//자식 노드 리스트
	vector<myNode*>			m_ChildList;
	//꼭지점 정점 리스트
	vector<PNCT_VERTEX>		m_CornerList;
	//꼭지점 정점 인덱스 리스트
	vector<DWORD>			m_CornerIndexList;
	//영역 안 오브젝트 리스트
	vector<myGameObject*>	m_ObjectList;
	//맵 인덱스 리스트
	vector<DWORD>			m_IndexList;
	//독자적인 버덱스 리스트
	vector<PNCT_VERTEX>		m_VertexList;
	//독자적인 버텍스 리스트
	vector<DWORD>			m_NodeIndexList;
	DWORD					m_dwFaceNum;
	ComPtr<ID3D11Buffer>	m_pIndexBuffer;
public:
	
public:
	myNode();
	virtual ~myNode();
};

