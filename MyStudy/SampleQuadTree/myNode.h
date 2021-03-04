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
	vector<Vector3>			m_CornerList;
	vector<myGameObject*>	m_ObjectList;
	vector<DWORD>			m_IndexNumList;
	vector<DWORD>			m_IndexList;
	ComPtr<ID3D11Buffer>	m_pIndexBuffer;
public:

public:
	myNode();
	virtual ~myNode();
};

