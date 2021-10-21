#pragma once
#include "myGraphics.h"

class myNode
{
public:
	//�ӽ�
	vector<int>				m_iObjectList;
public:
	myNode*					m_pParent;
	//����
	int						m_iDepth;
	//���ϴ� ����ΰ�
	bool					m_isLeaf;
	// �ٿ�� �ڽ�
	MY_BOX					m_myBox;
	MY_SPHERE				m_mySphere;
	vector<myNode*>			m_ChildList;
	vector<PNCT_VERTEX>		m_CornerList;
	vector<myNode*>			m_NeighborList;
	vector<DWORD>			m_CornerIndexList;
	//�ε��� ����Ʈ
	vector<DWORD>			m_IndexList;
	//�������� ������ ����Ʈ
	vector<PNCT_VERTEX>		m_VertexList;
	//�������� ���ؽ� ����Ʈ
	vector<DWORD>			m_NodeIndexList;
	DWORD					m_dwFaceNum;
	ComPtr<ID3D11Buffer>	m_pIndexBuffer;
public:

public:
	myNode();
	virtual ~myNode();
};

