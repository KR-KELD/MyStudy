#include "myGameObject.h"

class CRenderComponent : public myComponent
{
public:
	DEFINE_COMPONENT(CRenderComponent, myComponent, true);
};
DECLARE_COMPONENT(CRenderComponent);

class CImageRenderComponent : public CRenderComponent
{
public:
	DEFINE_COMPONENT(CImageRenderComponent, CRenderComponent, false);
};
DECLARE_COMPONENT(CImageRenderComponent);

class CPositionComponent : public myComponent
{
public:
	DEFINE_COMPONENT(CPositionComponent, myComponent, true);
};
DECLARE_COMPONENT(CPositionComponent);

class CRelativePositionComponent : public CPositionComponent
{
public:
	DEFINE_COMPONENT(CRelativePositionComponent, CPositionComponent, false);
};
DECLARE_COMPONENT(CRelativePositionComponent);
class CAbsolutePositionComponent : public CPositionComponent
{
public:
	DEFINE_COMPONENT(CAbsolutePositionComponent, CPositionComponent, false);
};
DECLARE_COMPONENT(CAbsolutePositionComponent);

int main()
{
	myGameObject* object = myGameObject::CreateGameObject();
	object->insertComponent(new CImageRenderComponent);
	object->insertComponent(new CAbsolutePositionComponent);

	cout << object->getComponent<CRenderComponent>()->getComponentName_() << endl;
	cout << object->getComponent<CPositionComponent>()->getComponentName_() << endl;
	cout << object->getComponent<CRenderComponent>()->getComponentId() << endl;
	cout << object->getComponent<CImageRenderComponent>()->getComponentId() << endl;

	delete object;
	return 0;
}

