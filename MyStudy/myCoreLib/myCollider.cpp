#include "myCollider.h"

DECLARE_COMPONENT(myBoxCollider);
DECLARE_COMPONENT(mySphereCollider);

bool myBoxCollider::Init()
{
	myCollider::Init();
	return true;
}

bool myBoxCollider::Frame()
{
	return true;
}

bool myBoxCollider::Render()
{
	return true;
}

bool myBoxCollider::Release()
{
	return true;
}

myBoxCollider::myBoxCollider()
{
	m_eType = COLLIDER_BOX;
}

myBoxCollider::~myBoxCollider()
{
}


bool mySphereCollider::Init()
{
	myCollider::Init();
	return true;
}

bool mySphereCollider::Frame()
{
	return true;
}

bool mySphereCollider::Render()
{
	return true;
}

bool mySphereCollider::Release()
{
	return true;
}

mySphereCollider::mySphereCollider()
{
	m_eType = COLLIDER_SPHERE;
}

mySphereCollider::~mySphereCollider()
{
}
