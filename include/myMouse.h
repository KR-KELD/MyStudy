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
	//��ũ������ ���콺 ��ǥ�� ���̷� ��ȯ���ִ� �Լ�
	void ScreenToRay();
	//�� ����� ���콺 ���� �浹 ���θ� �Ǻ��ϴ� �Լ� 
	bool PickingFace(myNode* pNode);
	bool PickingFace(myNode* pNode, myMap* pMap);
	//�� ����� �ٿ���ڽ� �浹 ���θ� �Ǻ��ϴ� �Լ�
	bool PickingAABBBox(myNode* pNode);

public:
	myMouse();
	virtual ~myMouse();
};

