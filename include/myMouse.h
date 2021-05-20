#pragma once
#include "myCameraManager.h"
#include "myCollision.h"
#include "myInput.h"
#include "myNode.h"
#include "myMap.h"

class myMouse
{
public:
	MY_RAY		m_myRay;
	Vector3		m_vIntersectionPos;
	Vector3		m_vPrevPos;
	float		m_fRange;
public:
	void ScreenToRay();
	bool PickingFace(myNode* pNode);
	bool PickingFace(myNode* pNode, myMap* pMap);
	bool PickingAABBBox(myNode* pNode);

public:
	myMouse();
	virtual ~myMouse();
};

