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
	//����� �ε��� ���� ��
	int						m_iIndexWidth;
	//����� �ε��� ���� ��
	int						m_iIndexHeight;
	// �ٿ�� �ڽ�
	MY_BOX					m_myBox;
	//���� ��� ����Ʈ
	vector<myNode*>			m_NeighborList;
	//�ڽ� ��� ����Ʈ
	vector<myNode*>			m_ChildList;
	//������ ���� ����Ʈ
	vector<PNCT_VERTEX>		m_CornerList;
	//������ ���� �ε��� ����Ʈ
	vector<DWORD>			m_CornerIndexList;
	//���� �� ������Ʈ ����Ʈ
	vector<myGameObject*>	m_ObjectList;
	//�� �ε��� ����Ʈ
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

