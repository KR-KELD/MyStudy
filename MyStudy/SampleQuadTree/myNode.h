#pragma once
#include "myGraphics.h"

class myNode
{
public:
	//����
	DWORD					m_dwDepth;
	//���ϴ� ����ΰ�
	BOOL					m_isLeaf;
	// �ٿ�� �ڽ�
	MY_BOX					m_myBox;
	vector<myNode*>			m_ChildList;
	vector<Vector3>			m_vCornerList;
	vector<myGameObject*>	m_ObjectList;
public:

public:
	myNode();
	virtual ~myNode();
};

