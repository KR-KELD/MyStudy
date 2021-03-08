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
	float		m_fRange;
public:
	void ScreenToRay();
	void MousePicking(myMap* pMap);
	bool InterSection(Vector3 vSegStart, Vector3 vSegEnd, Vector3 vFaceNormal,
					Vector3 vFaceV1, Vector3 vFaceV2, Vector3 vFaceV3);
	bool DiscribeFace(Vector3 vPos, Vector3 vFaceNormal, Vector3 vFaceV1,
					Vector3 vFaceV2, Vector3 vFaceV3);
public:
	myMouse();
	virtual ~myMouse();
};

