#pragma once
#include "myNode.h"
#include "myMap.h"
#include "myCameraManager.h"
#include "myDepthMap.h"
#include "myDxState.h"
class myQuadTree
{
public:
	//��
	myMap*			m_pMap;
	//�ֻ��� ���
	myNode*			m_pRootNode;
	//�ִ� ���� �Ѱ�
	int				m_iMaxdepth;
	//�ø� ī�޶�
	myCamera*		m_pCullingCamera;
	//����Ʈ ��ġ, ��Ʈ���� ����
	myCamera*		m_pLight;
	//������ ��ü
	myDepthMap*		m_pDepthMap;
	//������ ��� ����Ʈ
	vector<myNode*> m_LeafNodeList;
	//�׷��� ��� ����Ʈ
	vector<myNode*>	m_DrawNodeList;
	myShapeLine*	m_pQuadTreeLine;
public:
	//����Ʈ�� ����
	bool			CreateQuadTree(myMap* pMap);
	//����
	bool			Partition(myNode* pParentNode);

	//�������� �׷��� ����Ÿ�ٿ� �������� ������
	virtual bool	DepthRender(ID3D11DeviceContext*	pd3dContext);
	//������Ʈ�� ������ �����츦 ������
	virtual bool	ShadowRender(ID3D11DeviceContext*	pd3dContext);
	//����� ��� �ڽ� ��带 ������
	virtual bool	Draw(ID3D11DeviceContext*	pd3dContext, myNode* pNode);
	//�ø��� ��带 ������
	virtual bool	DrawCulling(ID3D11DeviceContext*	pd3dContext);
	//����Ʈ���� ���Ե� ������Ʈ�� ������
	bool			DrawObject(ID3D11DeviceContext*	pd3dContext);
	//������Ʈ �ø�
	virtual bool	CullingObject();
	//����Ʈ�� �ø�
	virtual bool	CullingVertex(ID3D11DeviceContext*	pd3dContext, myNode* pNode);
	//����Ʈ�� ����� ���ε� �ڽ� ����
	void			RepreshBindingObj(myNode* pNode);
	//����Ʈ�� ��� ����
	myNode*			CreateNode(myNode* pParentNode, DWORD LeftTop, DWORD RightTop, DWORD LeftBottom, DWORD RightBottom);
	bool			DrawNodeCollider(ID3D11DeviceContext*	pd3dContext, myNode* pNode);
	bool			DrawCollider(ID3D11DeviceContext*	pd3dContext);
public:
	virtual bool	Release();
	virtual bool	PreFrame();
	virtual bool	Frame();
	//�Ϲ� ������
	virtual bool	Render(ID3D11DeviceContext*	pd3dContext);
	//bool			AddObject(SampleIns* ins);
	//bool			RepreshQuadTreeObject();
public:
	myQuadTree();
	virtual ~myQuadTree();
};

