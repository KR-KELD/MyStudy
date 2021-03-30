#pragma once
#include "myModelGraphics.h"
#include "myAnimation.h"


class myModelObject : public myGameObject
{
public:
	DEFINE_COMPONENT(myModelObject, myGameObject, true);
public:
	ComPtr<ID3D11Buffer>		m_pBoneBuffer;
	//������ ���� �ᵵ ������� ���߿� �����ص� ��
	vector<myGameObject*>		m_myNodeList;
	vector<Matrix>				m_nodeMatList;
public:
	myModelObject();
	virtual ~myModelObject();
};

