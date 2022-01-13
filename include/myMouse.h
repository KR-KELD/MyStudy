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
	//스크린상의 마우스 좌표를 레이로 변환해주는 함수
	void ScreenToRay();
	//맵 노드의 마우스 레이 충돌 여부를 판별하는 함수 
	bool PickingFace(myNode* pNode);
	bool PickingFace(myNode* pNode, myMap* pMap);
	//맵 노드의 바운딩박스 충돌 여부를 판별하는 함수
	bool PickingAABBBox(myNode* pNode);

public:
	myMouse();
	virtual ~myMouse();
};

