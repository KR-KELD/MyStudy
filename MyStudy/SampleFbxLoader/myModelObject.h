#pragma once
#include "myModelGraphics.h"
#include "myAnimation.h"


class myModelObject : public myGameObject
{
public:
	DEFINE_COMPONENT(myModelObject, myGameObject, true);
public:
	ComPtr<ID3D11Buffer>		m_pBoneBuffer;
	//기존의 맵을 써도 상관없음 나중에 수정해도 됨
	vector<myGameObject*>		m_myNodeList;
	vector<Matrix>				m_nodeMatList;
public:
	myModelObject();
	virtual ~myModelObject();
};

