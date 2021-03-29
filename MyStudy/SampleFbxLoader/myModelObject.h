#pragma once
#include "myGameObject.h"
#include "myAnimation.h"

class myModelObject : public myGameObject
{
public:
	DEFINE_COMPONENT(myModelObject, myGameObject, true);
public:
	myAnimation					m_Animation;
	ComPtr<ID3D11Buffer>		m_BoneBuffer;
	vector<myGameObject*>		m_myNodeList;
public:
	myModelObject();
	~myModelObject();
};

