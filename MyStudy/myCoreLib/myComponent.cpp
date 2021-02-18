#include "myComponent.h"
DECLARE_COMPONENT(myComponent);
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

myComponent::myComponent()
{
}

myComponent::~myComponent()
{
}
