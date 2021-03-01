#pragma once
#include "myGraphics.h"


//�ڽ��� ������ �ִ� ������Ʈ
class myBaseObj
{
public:
	MY_BOX				m_myBox;
public:
	myBaseObj() {}
	virtual ~myBaseObj() {}
};
//�ڽ��� ���� ������ �÷��� �ִ� ������Ʈ
class MY_BoxObject : public myBaseObj
{
public:
	Matrix			m_matWorld;
	Vector3			m_vPosition;
	Vector4			m_vColor;
public:
	MY_BoxObject();
};

class myNode
{
public:
	//����
	DWORD				m_dwDepth;
	//���ϴ� ����ΰ�
	BOOL				m_isLeaf;
	//Ÿ���ΰ�
	BOOL				m_isTile;
	// �ٿ�� �ڽ�
	MY_BOX				m_myBox;
	vector<DWORD>		m_CornerIndex;
	// ����� �ڳ� ���� �ε��� �� ��ġ
	vector<Vector3>		m_CornerList;
	//�ڳʸ���Ʈ(?)
	vector<myNode*>		m_ChildList;
	//������� ����Ʈ
	vector<myNode*>		m_NeighborList;
	//������Ʈ ����Ʈ(?)
	vector<myBaseObj*>	m_ObjectList;
	// �߰�
	DWORD				m_dwPositionIndex[2];// X,Y	
	//���ؽ����� ��
	D3D11_BOX			m_BoxVB;
	//�ε������� ��
	D3D11_BOX			m_BoxIB;
	//���ؽ����� ������
	UINT				m_iBeginVB;
	//�ε������� ������
	UINT				m_iBeginIB;
	//�ε��� ����Ʈ
	vector<DWORD>		m_IndexList;
	//���ؽ� ����Ʈ
	vector <PNCT_VERTEX>	m_VertexList;
	//���ؽ����� �������̽�
	ComPtr<ID3D11Buffer>	m_pVertexBuffer;
	//�ε������� �������̽�
	ComPtr<ID3D11Buffer>	m_pIndexBuffer;
public:
	myNode();
	virtual ~myNode();
};

