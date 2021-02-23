#include "myCamera.h"
//#include "myInput.h"
DECLARE_COMPONENT(myCamera);
bool		myCamera::CreateViewMatrix(
	Vector3 p,
	Vector3 t,
	Vector3 u)
{
	m_pTransform->m_vPos = p;
	m_pTransform->m_vTarget = t;
	m_fDistance = (m_pTransform->m_vPos - m_pTransform->m_vTarget).Length();
	m_matView = Matrix::CreateLookAt(p, t, u);

	Matrix mInvView;
	mInvView = m_matView.Invert();
	Vector3* pZBasis = (Vector3*)&mInvView._31;

	m_pTransform->m_vRot.y = atan2f(pZBasis->x, pZBasis->z);
	float fLen = sqrtf(pZBasis->z * pZBasis->z + pZBasis->x * pZBasis->x);
	m_pTransform->m_vRot.x = -atan2f(pZBasis->y, fLen);

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
bool myCamera::CreateOrthographic(float width, float height, float zNearPlane, float zFarPlane)
{
	m_matProj = Matrix::CreateOrthographic(
		width, height,
		zNearPlane, zFarPlane);
	return true;
}
void myCamera::SetPos(Vector3 p)
{
	m_pTransform->m_vPos = p;
}
void myCamera::SetTarget(Vector3 p)
{
	m_pTransform->m_vTarget = p;
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
	float fHeight = m_pTransform->m_vPos.y;
	m_pTransform->m_vPos = m_pTransform->m_vTarget - vLocalLook * m_fDistance;
	m_pTransform->m_vPos.y = fHeight;
}
void myCamera::FrontMovement(float fDir)
{
	Vector3 vOffset = m_pTransform->m_vLook * g_fSecondPerFrame * m_pSpeed * fDir;
	m_pTransform->m_vPos += vOffset;
}
void myCamera::RightMovement(float fDir)
{
	Vector3 vMove = m_pTransform->m_vRight * g_fSecondPerFrame * m_pSpeed * fDir;
	m_pTransform->m_vPos += vMove;
}
void myCamera::UpMovement(float fDir)
{
	Vector3 vMove = m_pTransform->m_vUp * g_fSecondPerFrame * m_pSpeed * fDir;
	m_pTransform->m_vPos += vMove;
}
void myCamera::FrontBase(float fDir)
{
	Vector3 vSide = { 0,0,1 };
	Vector3 vMove = vSide * g_fSecondPerFrame * m_pSpeed * fDir;
	m_pTransform->m_vPos += vMove;
	m_pTransform->m_vTarget += m_pTransform->m_vLook * m_pSpeed;
}
void myCamera::RightBase(float fDir)
{
	Vector3 vSide = { 1,0,0 };
	Vector3 vMove = vSide * g_fSecondPerFrame * m_pSpeed * fDir;
	m_pTransform->m_vPos += vMove;
	m_pTransform->m_vTarget += m_pTransform->m_vLook * m_pSpeed;
}
void myCamera::UpBase(float fDir)
{
	Vector3 vUp = { 0,1,0 };
	Vector3 vMove = vUp * g_fSecondPerFrame * m_pSpeed * fDir;
	m_pTransform->m_vPos += vMove;
	m_pTransform->m_vTarget += m_pTransform->m_vLook * m_pSpeed;
}
void myCamera::UpdateVector()
{
	m_pTransform->m_vLook.x = m_matView._13;
	m_pTransform->m_vLook.y = m_matView._23;
	m_pTransform->m_vLook.z = m_matView._33;
	m_pTransform->m_vUp.x = m_matView._12;
	m_pTransform->m_vUp.y = m_matView._22;
	m_pTransform->m_vUp.z = m_matView._32;
	m_pTransform->m_vRight.x = m_matView._11;
	m_pTransform->m_vRight.y = m_matView._21;
	m_pTransform->m_vRight.z = m_matView._31;

	m_pTransform->m_vLook.Normalize();
	m_pTransform->m_vUp.Normalize();
	m_pTransform->m_vRight.Normalize();



}
bool myCamera::Init()
{
	return true;
}
bool myCamera::Frame()
{
	Vector3 vUp = { 0,1,0 };
	m_matView = Matrix::CreateLookAt(
		m_pTransform->m_vPos,
		m_pTransform->m_vTarget,
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
		m_rtOffset.left = 0;
		m_rtOffset.bottom = 0;
	}return 0;
	case WM_MOUSEMOVE:
	{
		if (m_bDrag)
		{
			m_rtOffset.left = m_ptClick.x - LOWORD(lParam);
			m_rtOffset.bottom = m_ptClick.y - HIWORD(lParam);
			m_ptClick.x = LOWORD(lParam);
			m_ptClick.y = HIWORD(lParam);
		}
	}return 0;
	case WM_LBUTTONUP:
	{
		m_bDrag = false;
		m_rtOffset.left = 0;
		m_rtOffset.bottom = 0;
	}return 0;
	case WM_MOUSEWHEEL:
	{
		m_fWheelDelta += GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA;
	}break;
	}
	return -1;
}
bool myCamera::DrawFrustum(ID3D11DeviceContext * pd3dContext, Matrix * pmatView, Matrix * pmatProj)
{
	return true;
}
myCamera::myCamera()
{
	m_pSpeed = 30.0f;
	m_bDrag = false;
	m_fWheelDelta = 0;
}
myCamera::~myCamera()
{

}