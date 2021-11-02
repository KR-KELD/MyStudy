#pragma once
#include "myCore.h"
#include "myMap.h"

class Sample : public myCore
{
public:
	myFbxObj* m_pFbxObj;
	shared_ptr<myModelObject> m_pModelObj;
	myMap*	m_pMap;
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
};