#pragma once
#include "myGraphics.h"

class myNode
{
public:
	myNode*					m_pParent;
	//����
	int						m_iDepth;
	//���ϴ� ����ΰ�
	bool					m_isLeaf;
	// �ٿ�� �ڽ�
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

