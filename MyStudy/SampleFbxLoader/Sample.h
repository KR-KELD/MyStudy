#pragma once
#include "myCore.h"
#include "myModelViewCamera.h"
#include "myHeightMap.h"
#include "myMiniMap.h"
#include "myQuadTree.h"
#include "myFbxLoader.h"
#include "myMouse.h"
#include "myModelGraphics.h"
class Sample : public myCore
{
public:
	myFbxObj* m_pFbxObj;
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
};

