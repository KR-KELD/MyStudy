#pragma once
#include "myGraphics.h"
#include "myModelObject.h"

typedef vector<DWORD> DWORD_VECTOR;

struct myLodPatch
{
	UINT iLodLevel;
	DWORD_VECTOR		 IndexList[16];
	ComPtr<ID3D11Buffer> IndexBufferList[16];
};

class myNode
{
public:
	//�ӽ�
	vector<SampleIns*>		m_ObjectList;
public:
	myNode*					m_pParent;
	//����
	int						m_iDepth;
	int						m_iIndex;
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
	bool			IsInRect(Vector3& vCenter);
public:
	myNode();
	virtual ~myNode();
};

