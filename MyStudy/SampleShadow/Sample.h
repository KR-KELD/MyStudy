#pragma once
#include "myCore.h"
#include "myMap.h"
#include "myDxRT.h"
#include "myMiniMap.h"
#include "myDepthMap.h"

struct cbShadowMatrix
{
	Matrix g_matShadow;
};

class Sample : public myCore
{
public:
	myFbxObj* m_pFbxObj;
	shared_ptr<myModelObject> m_pModelObj;
	myMap*	m_pMap;
	myCamera* m_pLight;

	myMiniMap* m_pShadowMinimap;
	myDepthMap* m_pDepthMap;

public:


	bool Init() override;
	bool Frame() override;
	bool PreFrame() override;
	bool Render() override;
	bool Release() override;
};