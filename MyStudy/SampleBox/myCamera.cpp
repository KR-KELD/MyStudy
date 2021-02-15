#include "myCamera.h"
#include "myInput.h"
bool		myCamera::CreateViewMatrix(
	Vector3 p,
	Vector3 t,
	Vector3 u)
{
	m_vCameraPos = p;
	m_vCameraTarget = t;
	m_fDistance = (m_vCameraPos - m_vCameraTarget).Length();
	m_matView = Matrix::CreateLookAt(p, t, u);
	UpdateVector();
	return true;
}
bool		myCamera::CreateProjMatrix(
	float fN,
	float fF,
	float fFov,
	float fAspect)
{
	m_matProj = Matrix::CreatePerspectiveFieldOfView(
		fFov, fAspect, fN, fF);
	return true;
}
void myCamera::SetPos(Vector3 p)
{
	m_vCameraPos = p;
}
void myCamera::SetTarget(Vector3 p)
{
	m_vCameraTarget = p;
}
void myCamera::Update(Vector4 data)
{
	Matrix matRotation;
	matRotation = Matrix::CreateRotationY(data.y);
	Vector3 vLocalUp = { 0,1,0 };
	Vector3 vLocalLook = { 0,0,1 };
	vLocalUp = Vector3::Transform(vLocalUp, matRotation);
	vLocalLook = Vector3::Transform(vLocalLook, matRotation);
	vLocalLook.Normalize();
	vLocalUp.Normalize();
	float fHeight = m_vCameraPos.y;
	m_vCameraPos = m_vCameraTarget - vLocalLook * m_fDistance;
	m_vCameraPos.y = fHeight;
}
void myCamera::FrontMovement(float fDir)
{
	Vector3 vOffset = m_vLook * g_fSecondPerFrame * m_pSpeed * fDir;
	m_vCameraPos += vOffset;
}
void myCamera::RightMovement(float fDir)
{
	Vector3 vMove = m_vRight * g_fSecondPerFrame * m_pSpeed * fDir;
	m_vCameraPos += vMove;
}
void myCamera::UpMovement(float fDir)
{
	Vector3 vMove = m_vUp * g_fSecondPerFrame * m_pSpeed * fDir;
	m_vCameraPos += vMove;
}
void myCamera::FrontBase(float fDir)
{
	Vector3 vSide = { 0,0,1 };
	Vector3 vMove = vSide * g_fSecondPerFrame * m_pSpeed * fDir;
	m_vCameraPos += vMove;
	m_vCameraTarget += m_vLook * m_pSpeed;
}
void myCamera::RightBase(float fDir)
{
	Vector3 vSide = { 1,0,0 };
	Vector3 vMove = vSide * g_fSecondPerFrame * m_pSpeed * fDir;
	m_vCameraPos += vMove;
	m_vCameraTarget += m_vLook * m_pSpeed;
}
void myCamera::UpBase(float fDir)
{
	Vector3 vUp = { 0,1,0 };
	Vector3 vMove = vUp * g_fSecondPerFrame * m_pSpeed * fDir;
	m_vCameraPos += vMove;
	m_vCameraTarget += m_vLook * m_pSpeed;
}
void myCamera::UpdateVector()
{
	m_vLook.x = m_matView._13;
	m_vLook.y = m_matView._23;
	m_vLook.z = m_matView._33;
	m_vUp.x = m_matView._12;
	m_vUp.y = m_matView._22;
	m_vUp.z = m_matView._32;
	m_vRight.x = m_matView._11;
	m_vRight.y = m_matView._21;
	m_vRight.z = m_matView._31;
}
bool myCamera::Frame()
{
	Vector3 vUp = { 0,1,0 };
	m_matView = Matrix::CreateLookAt(
		m_vCameraPos,
		m_vCameraTarget,
		vUp);
	UpdateVector();
	return true;
}
int myCamera::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		m_bDrag = true;
		m_ptClick.x = LOWORD(lParam);
		m_ptClick.y = HIWORD(lParam);
		m_rtOffset.x = 0;
		m_rtOffset.y = 0;
	}return 0;
	case WM_MOUSEMOVE:
	{
		if (m_bDrag)
		{
			m_rtOffset.x = m_ptClick.x - LOWORD(lParam);
			m_rtOffset.y = m_ptClick.y - HIWORD(lParam);
			m_ptClick.x = LOWORD(lParam);
			m_ptClick.y = HIWORD(lParam);
		}
	}return 0;
	case WM_LBUTTONUP:
	{
		m_bDrag = false;
		m_rtOffset.x = 0;
		m_rtOffset.y = 0;
	}return 0;
	}
	return -1;
}
myCamera::myCamera()
{
	m_pSpeed = 30.0f;
	m_bDrag = false;
}
myCamera::~myCamera()
{

}