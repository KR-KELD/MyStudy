#pragma once
#include "myFrustum.h"
class myCamera : public myComponent
{
public:
	DEFINE_COMPONENT(myCamera, myComponent, true)
public:
	myFrustum				m_Frustum;
	float					m_fDistance;
	bool					m_bDrag;
	POINT					m_ptClick;
	RECT					m_rtOffset;
	RECT					m_rtPreOffset;
	float					m_fWheelDelta;
	POINT					m_ptPrePosition;
public:
	virtual void Update(Vector4 d);
	virtual bool		CreateViewMatrix(
		Vector3 p,
		Vector3 t,
		Vector3 u = { 0,1,0 });
	virtual bool		CreateProjMatrix(
		float fN,
		float fF,
		float fFov,
		float fAspect
	);
	virtual bool		CreateOrthographic(
		float width, float height,
		float zNearPlane, float zFarPlane);
	virtual bool CreateFrustum();
	virtual bool PostInit() { return true; }
	virtual myComponent* Clone();
	virtual bool Init();
	virtual bool Frame();
	virtual bool Release();
	virtual  int WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual bool FrameFrustum();
	virtual bool DrawFrustum(ID3D11DeviceContext*	pd3dContext, Matrix* pmatView, Matrix* pmatProj);
public:
	myCamera();
	virtual ~myCamera();
};

