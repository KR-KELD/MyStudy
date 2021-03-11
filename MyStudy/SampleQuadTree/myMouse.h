#pragma once
#include "myStd.h"
#include "myCameraManager.h"
#include "myInput.h"
#include "myMap.h"
#include "myCollision.h"
#include "myNode.h"

class myMouse
{
public:
	MY_RAY		m_myRay;
	Vector3		m_vIntersectionPos;
	float		m_fRange;
public:
	void ScreenToRay();
	bool PickingFace(myMap* pMap, myNode* pNode);
	bool PickingAABBBox(myNode* pNode);

public:
	myMouse();
	virtual ~myMouse();
};

