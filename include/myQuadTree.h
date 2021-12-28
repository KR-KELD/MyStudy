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

	//�ѷ��� ���
	vector<myNode*> m_LeafNodeList;
	vector<myNode*>	m_DrawNodeList;
public:
	//����Ʈ�� ����
	bool			CreateQuadTree(myMap* pMap);
	//����
	bool			Partition(myNode* pParentNode);
	virtual bool	Release();
	virtual bool	PreFrame();
	virtual bool	Frame();
	//�Ϲ� ������
	virtual bool	Render(ID3D11DeviceContext*	pd3dContext);
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
	void			RepreshBindingObj(myNode* pNode);
	myNode*			CreateNode(myNode* pParentNode, DWORD LeftTop, DWORD RightTop, DWORD LeftBottom, DWORD RightBottom);
public:
	//�ӽ�
	bool			AddObject(SampleIns* ins);
	bool			RepreshQuadTreeObject();
public:
	myQuadTree();
	virtual ~myQuadTree();
};

