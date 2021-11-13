#pragma once
#include "myNode.h"
#include "myMap.h"
#include "myCameraManager.h"
#include "myDepthMap.h"
class myQuadTree
{
public:
	myMap*			m_pMap;
	//�ֻ��� ���
	myNode*			m_pRootNode;
	//�ִ� ���� �Ѱ�
	int				m_iMaxdepth;
	//�ּ� ũ��
	bool			m_isShadow;
	myCamera*		m_pCullingCamera;
	myCamera*		m_pLight;
	myDepthMap*		m_pDepthMap;

	//�ѷ��� ���
	vector<myNode*> m_LeafNodeList;
	vector<myNode*>	m_DrawNodeList;
public:
	bool			CreateQuadTree(myMap* pMap);
	//����
	bool			Partition(myNode* pParentNode);
	virtual bool	Release();
	virtual bool	PreFrame();
	virtual bool	Frame();
	virtual bool	Render(ID3D11DeviceContext*	pd3dContext);
	virtual bool	DepthRender(ID3D11DeviceContext*	pd3dContext);
	virtual bool	ShadowRender(ID3D11DeviceContext*	pd3dContext);
	virtual bool	Draw(ID3D11DeviceContext*	pd3dContext, myNode* pNode);
	virtual bool	DrawCulling(ID3D11DeviceContext*	pd3dContext);
	bool			DrawObject(ID3D11DeviceContext*	pd3dContext);
	virtual bool	CullingNode();
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

