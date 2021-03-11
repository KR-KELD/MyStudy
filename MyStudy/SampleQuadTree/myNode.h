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
	vector<PNCT_VERTEX>		m_CornerList;
	vector<DWORD>			m_CornerIndexList;
	vector<myGameObject*>	m_ObjectList;
	//�������� �ε��� ����Ʈ
	vector<DWORD>			m_IndexList;
	//�������� ������ ����Ʈ
	vector<PNCT_VERTEX>		m_VertexList;
	DWORD					m_dwFaceNum;
	ComPtr<ID3D11Buffer>	m_pIndexBuffer;
public:
	
public:
	myNode();
	virtual ~myNode();
};

