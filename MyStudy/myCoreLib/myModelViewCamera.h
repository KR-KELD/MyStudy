#pragma once
#include "myCamera.h"
#include "myFrustum.h"

class myArcball
{
public:
	Vector3   m_vDownPt;
	Vector3   m_vCurrentPt;
	Quaternion  m_qDown;
	Quaternion  m_qNow;
	bool		m_bDrag;
public:
	Matrix		GetRotationMatrix();
	Quaternion  QuatFromPoints(Vector3   v0, Vector3   v1);
	Vector3		ScreenToVector(float x, float y);
	void		OnBegin(int x, int y);
	void		OnMove(int x, int y);
	void		OnEnd();
public:
	myArcball() 
	{
		m_qNow = Quaternion::Identity;
		m_qDown = Quaternion::Identity;
		m_bDrag = false;
	}
};

class myModelViewCamera : public myCamera
{
public:
	//Matrix    m_matWorld;
	//myFrustum*  m_Frustum;
public:
	myArcball	m_WorldArcball;
	myArcball	m_ViewArcball;
	Matrix		m_mModelLastRot;
public:

	virtual  int WndProc(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam);
public:
	virtual bool PostInit() override;
	virtual void Update(Vector4 data) override;
	virtual bool Frame() override;
	virtual bool DrawFrustum(Matrix* pmatView, Matrix* pmatProj);
	virtual bool FrameFrustum();
public:
	myModelViewCamera();
	virtual ~myModelViewCamera();
};

