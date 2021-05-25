#pragma once
#include "myGameObject.h"
#include "myBaseObject.h"

class myBoxCollider : public myCollider
{
public:
	DEFINE_COMPONENT(myBoxCollider, myCollider, true)
public:
	MY_BOX				m_Box;
public:
	virtual bool		Init();
	virtual bool		Frame();
	virtual bool		Render(ID3D11DeviceContext*	pd3dContext);
	virtual bool		Release();
public:
	myBoxCollider();
	virtual ~myBoxCollider();
};


class mySphereCollider : public myCollider
{
public:
	DEFINE_COMPONENT(mySphereCollider, myCollider, true)
public:
	MY_SPHERE			m_Sphere;
public:
	virtual bool		Init();
	virtual bool		Frame();
	virtual bool		Render(ID3D11DeviceContext*	pd3dContext);
	virtual bool		Release();
public:
	mySphereCollider();
	virtual ~mySphereCollider();
};

