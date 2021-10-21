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
	//임시
	vector<SampleIns*>		m_ObjectList;
public:
	myNode*					m_pParent;
	//깊이
	int						m_iDepth;
	int						m_iIndex;
	//최하단 노드인가
	bool					m_isLeaf;
	// 바운딩 박스
	MY_BOX					m_myBox;
	MY_SPHERE				m_mySphere;
	vector<myNode*>			m_ChildList;
	vector<PNCT_VERTEX>		m_CornerList;
	vector<myNode*>			m_NeighborList;
	vector<DWORD>			m_CornerIndexList;
	//인덱스 리스트
	vector<DWORD>			m_IndexList;
	//독자적인 버덱스 리스트
	vector<PNCT_VERTEX>		m_VertexList;
	//독자적인 버텍스 리스트
	vector<DWORD>			m_NodeIndexList;
	DWORD					m_dwFaceNum;
	ComPtr<ID3D11Buffer>	m_pIndexBuffer;
public:
	bool			IsInRect(Vector3& vCenter);
public:
	myNode();
	virtual ~myNode();
};

