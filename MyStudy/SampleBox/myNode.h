#pragma once
#include "myGraphics.h"

class myNode
{
public:
	//깊이
	DWORD				m_dwDepth;
	//최하단 노드인가
	BOOL				m_isLeaf;
	//타일인가
	BOOL				m_isTile;
	// 바운딩 박스
	MY_BOX				m_myBox;
	vector<DWORD>		m_CornerIndex;
	// 노드의 코너 정점 인덱스 및 위치
	vector<Vector3>		m_CornerList;
	//코너리스트(?)
	vector<myNode*>		m_ChildList;
	//인접노드 리스트
	vector<myNode*>		m_NeighborList;
	//오브젝트 리스트(?)
	vector<myGameObject*>	m_ObjectList;
	// 추가
	DWORD				m_dwPositionIndex[2];// X,Y	
	//버텍스버퍼 값
	D3D11_BOX			m_BoxVB;
	//인덱스버퍼 값
	D3D11_BOX			m_BoxIB;
	//버텍스버퍼 시작점
	UINT				m_iBeginVB;
	//인덱스버퍼 시작점
	UINT				m_iBeginIB;
	//인덱스 리스트
	vector<DWORD>		m_IndexList;
	//버텍스 리스트
	vector <PNCT_VERTEX>	m_VertexList;
	//버텍스버퍼 인터페이스
	ComPtr<ID3D11Buffer>	m_pVertexBuffer;
	//인덱스버퍼 인터페이스
	ComPtr<ID3D11Buffer>	m_pIndexBuffer;
public:
	myNode();
	virtual ~myNode();
};

