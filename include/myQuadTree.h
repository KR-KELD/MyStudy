#pragma once
#include "myNode.h"
#include "myMap.h"
#include "myCameraManager.h"
#include "myDepthMap.h"
#include "myDxState.h"
class myQuadTree
{
public:
	//맵
	myMap*			m_pMap;
	//최상위 노드
	myNode*			m_pRootNode;
	//최대 분할 한계
	int				m_iMaxdepth;
	//컬링 카메라
	myCamera*		m_pCullingCamera;
	//라이트 위치, 매트릭스 정보
	myCamera*		m_pLight;
	//뎁스맵 객체
	myDepthMap*		m_pDepthMap;

	//뿌려줄 노드
	vector<myNode*> m_LeafNodeList;
	vector<myNode*>	m_DrawNodeList;
public:
	//쿼드트리 생성
	bool			CreateQuadTree(myMap* pMap);
	//분할
	bool			Partition(myNode* pParentNode);
	virtual bool	Release();
	virtual bool	PreFrame();
	virtual bool	Frame();
	//일반 랜더링
	virtual bool	Render(ID3D11DeviceContext*	pd3dContext);
	//뎁스맵을 그려줄 랜더타겟에 뎁스맵을 랜더링
	virtual bool	DepthRender(ID3D11DeviceContext*	pd3dContext);
	//오브젝트와 뎁스맵 쉐도우를 랜더링
	virtual bool	ShadowRender(ID3D11DeviceContext*	pd3dContext);
	//노드의 모든 자식 노드를 랜더링
	virtual bool	Draw(ID3D11DeviceContext*	pd3dContext, myNode* pNode);
	//컬링된 노드를 랜더링
	virtual bool	DrawCulling(ID3D11DeviceContext*	pd3dContext);
	//쿼드트리에 포함된 오브젝트를 랜더링
	bool			DrawObject(ID3D11DeviceContext*	pd3dContext);
	//오브젝트 컬링
	virtual bool	CullingObject();
	//쿼드트리 컬링
	virtual bool	CullingVertex(ID3D11DeviceContext*	pd3dContext, myNode* pNode);
	void			RepreshBindingObj(myNode* pNode);
	myNode*			CreateNode(myNode* pParentNode, DWORD LeftTop, DWORD RightTop, DWORD LeftBottom, DWORD RightBottom);
public:
	//임시
	bool			AddObject(SampleIns* ins);
	bool			RepreshQuadTreeObject();
public:
	myQuadTree();
	virtual ~myQuadTree();
};

