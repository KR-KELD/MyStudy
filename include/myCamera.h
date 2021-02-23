#pragma once
#include "myGameObject.h"
class myCamera : public myComponent
{
public:
	DEFINE_COMPONENT(myCamera, myComponent, true);
public:
	float			m_pSpeed;
	//Vector3		m_vCameraPos = { 10,0,-10 };
	//Vector3		m_vCameraTarget = { 0,0,0 };
	Matrix			m_matWorld;
	Matrix			m_matView;
	Matrix			m_matProj;
	//Vector3			m_vLook;
	//Vector3			m_vUp;
	//Vector3			m_vRight;
	float			m_fDistance;
	bool			m_bDrag;
	//POINT			m_ptCurrentPos;
	//POINT			m_ptOffset;
	//POINT			m_ptPrePos;
	POINT			m_ptClick;
	RECT			m_rtOffset;
	RECT			m_rtPreOffset;
	int				m_fWheelDelta;
	//Vector4			m_vDirValue;
	POINT			m_ptPrePosition;
public:
	virtual void SetPos(Vector3 p);
	virtual void SetTarget(Vector3 p);
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

	void FrontMovement(float fDir = 1.0f);
	void RightMovement(float fDir = 1.0f);
	void UpMovement(float fDir = 1.0f);
	void FrontBase(float fDir);
	void RightBase(float fDir);
	void UpBase(float fDir);
	virtual void UpdateVector();
	virtual bool PostInit() { return true; }
	virtual bool Init();
	virtual bool Frame();
	virtual  int WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual bool FrameFrustum(ID3D11DeviceContext*	pd3dContext)
	{
		return true;
	};
	virtual bool DrawFrustum(ID3D11DeviceContext*	pd3dContext,
		Matrix* pmatView, Matrix* pmatProj);
public:
	myCamera();
	virtual ~myCamera();
};

