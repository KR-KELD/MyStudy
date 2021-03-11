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
	// 바운딩 박스
	MY_BOX					m_myBox;
	vector<myNode*>			m_ChildList;
	vector<PNCT_VERTEX>		m_CornerList;
	vector<DWORD>			m_CornerIndexList;
	vector<myGameObject*>	m_ObjectList;
	//독자적인 인덱스 리스트
	vector<DWORD>			m_IndexList;
	//독자적인 버덱스 리스트
	vector<PNCT_VERTEX>		m_VertexList;
	DWORD					m_dwFaceNum;
	ComPtr<ID3D11Buffer>	m_pIndexBuffer;
public:
	
public:
	myNode();
	virtual ~myNode();
};

