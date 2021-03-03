#pragma once
#include "myGraphics.h"

class myNode
{
public:
	//깊이
	DWORD					m_dwDepth;
	//최하단 노드인가
	BOOL					m_isLeaf;
	// 바운딩 박스
	MY_BOX					m_myBox;
	vector<myNode*>			m_ChildList;
	vector<Vector3>			m_vCornerList;
	vector<myGameObject*>	m_ObjectList;
public:

public:
	myNode();
	virtual ~myNode();
};

