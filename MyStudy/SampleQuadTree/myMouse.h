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
		m_vDir = Vector3(0.0f,0.0f,1.0f);
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
	bool InterSection(Vector3& vSegStart, Vector3& vSegEnd, Vector3& vFaceNormal,
					Vector3& v1, Vector3& v2, Vector3& v3);
	bool DetermineFace(Vector3& vPos, Vector3& vFaceNormal, 
					Vector3& v1, Vector3& v2, Vector3& v3);
	bool InterSectDetermine(Vector3& vRayStart, Vector3& vRayDir,
					Vector3& v1, Vector3& v2, Vector3& v3,
					float* t, float* u, float* v);
public:
	myMouse();
	virtual ~myMouse();
};

