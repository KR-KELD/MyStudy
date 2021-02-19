#include "myGameObject.h"

DECLARE_COMPONENT(myComponent);
DECLARE_COMPONENT(myGameObject);
bool myComponent::Init()
{
	return true;
}

bool myComponent::PreFrame()
{
	return true;
}

bool myComponent::Frame()
{
	return true;
}

bool myComponent::PostFrame()
{
	return true;
}

bool myComponent::PreRender()
{
	return true;
}

bool myComponent::Render()
{
	return true;
}

bool myComponent::PostRender()
{
	return true;
}

void myComponent::Update()
{
}

void myComponent::Reset()
{

}

bool myComponent::Action()
{
	return true;
}

bool myComponent::Release()
{
	return true;
}

bool myGameObject::Init()
{
	return true;
}

bool myGameObject::PreFrame()
{
	return true;
}

bool myGameObject::Frame()
{
	return true;
}

bool myGameObject::PostFrame()
{
	return true;
}

bool myGameObject::PreRender()
{
	return true;
}

bool myGameObject::Render()
{
	return true;
}

bool myGameObject::PostRender()
{
	return true;
}

void myGameObject::Update()
{
}

void myGameObject::Reset()
{
}

bool myGameObject::Action()
{
	return true;
}

bool myGameObject::Release()
{
	for (m_iter = m_ComponentList.begin();
		m_iter != m_ComponentList.end();
		m_iter++)
	{
		if ((*m_iter).second == this)
		{
			continue;
		}
		(*m_iter).second->Release();
		delete (*m_iter).second;
	}
	m_ComponentList.clear();
	return true;
}

