#pragma once
#include "myMatrix.h"
class myCamera
{
public:
	float			m_pSpeed;
	myVector3		m_vCameraPos = { 10,0,-10 };
	myVector3		m_vCameraTarget = { 0,0,0 };
	myMatrix		m_matView;
	myMatrix		m_matProj;
	myVector3		m_vLook;
	myVector3		m_vUp;
	myVector3		m_vRight;
	float			m_fDistance;
public:
	virtual void SetPos(myVector3 p);
	virtual void SetTarget(myVector3 p);
	virtual void Update(myVector4 d);
	virtual bool		CreateViewMatrix(
		myVector3 p,
		myVector3 t,
		myVector3 u = { 0,1,0 });
	virtual bool		CreateProjMatrix(
		float fN,
		float fF,
		float fFov,
		float fAspect
	);
	void FrontMovement(float fDir = 1.0f);
	void RightMovement(float fDir = 1.0f);
	void UpMovement(float fDir = 1.0f);
	void FrontBase(float fDir);
	void RightBase(float fDir);
	void UpBase(float fDir);
	virtual void UpdateVector();
	virtual bool Frame();
public:
	myCamera();
	virtual ~myCamera();
};

