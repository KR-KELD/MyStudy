//#pragma once
//#include "myGraphics.h"
//
//class myBaseObj
//{
//public:
//	MY_BOX				m_myBox;
//public:
//	myBaseObj() {}
//	virtual ~myBaseObj() {}
//};
//class MY_BoxObject : public myBaseObj
//{
//public:
//	Matrix			m_matWorld;
//	Vector3			m_vPosition;
//	Vector4			m_vColor;
//public:
//	MY_BoxObject();
//};
//
//class myNode
//{
//public:
//	DWORD				m_dwDepth;
//	BOOL				m_isLeaf;
//	BOOL				m_isTile;
//	// 바운딩 박스
//	MY_BOX				m_myBox;
//	vector<DWORD>		m_CornerIndex;
//	// 노드의 코너 정점 인덱스 및 위치
//	vector<Vector3>		m_CornerList;
//	vector<myNode*>		m_ChildList;
//	vector<myNode*>		m_NeighborList;
//	vector<myBaseObj*>	m_ObjectList;
//	// 추가
//	DWORD				m_dwPositionIndex[2];// X,Y	
//	D3D11_BOX			m_BoxVB;
//	D3D11_BOX			m_BoxIB;
//	UINT				m_iBeginVB;
//	UINT				m_iBeginIB;
//	vector<DWORD>		m_IndexList;
//	vector <PNCT_VERTEX>	m_VertexList;
//	ComPtr<ID3D11Buffer>	m_pVertexBuffer;
//	ComPtr<ID3D11Buffer>	m_pIndexBuffer;
//public:
//	myNode();
//	virtual ~myNode();
//};
//
