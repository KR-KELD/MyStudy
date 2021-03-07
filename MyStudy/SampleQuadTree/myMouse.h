#pragma once
#include "myStd.h"
#include "myCameraManager.h"
#include "myInput.h"
#include "myMap.h"

struct myRay
{
	Vector3 m_vOrigin;
	Vector3 m_vDir;
	myRay()
	{
		m_vOrigin = Vector3::Zero;
		m_vDir = Vector3::Forward;
	}
};

class myMouse
{
public:
	myRay		m_myRay;
	Vector3		m_vIntersectionPos;
public:
	void ScreenToRay();
	void MousePicking(myMap* pMap);
	bool InterSection();
	bool DiscribeFace();
public:
	myMouse();
	virtual ~myMouse();
};

