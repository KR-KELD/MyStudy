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
	m_pTransform->m_matView = Matrix::CreateLookAt(p, t, u);

	Matrix mInvView;
	mInvView = m_pTransform->m_matView.Invert();
	Vector3* pZBasis = (Vector3*)&mInvView._31;

	m_pTransform->m_vRot.y = atan2f(pZBasis->x, pZBasis->z);
	float fLen = sqrtf(pZBasis->z * pZBasis->z + pZBasis->x * pZBasis->x);
	m_pTransform->m_vRot.x = -atan2f(pZBasis->y, fLen);

	m_pTransform->UpdateVector();
	return true;
}
bool		myCamera::CreateProjMatrix(
	float fN,
	float fF,
	float fFov,
	float fAspect)
{
	m_pTransform->m_matProj = Matrix::CreatePerspectiveFieldOfView(
		fFov, fAspect, fN, fF);
	return true;
}
bool myCamera::CreateOrthographic(float width, float height, float zNearPlane, float zFarPlane)
{
	m_pTransform->m_matProj = Matrix::CreateOrthographic(
		width, height,
		zNearPlane, zFarPlane);
	return true;
}
bool myCamera::CreateFrustum()
{
	m_Frustum.m_FrustumObj.m_pTransform = m_pTransform;
	m_Frustum.Create();
	return true;
}

void myCamera::Update(Vector4 data)
{
	//Matrix matRotation;
	//matRotation = Matrix::CreateRotationY(data.y);
	//Vector3 vLocalUp = { 0,1,0 };
	//Vector3 vLocalLook = { 0,0,1 };
	//vLocalUp = Vector3::Transform(vLocalUp, matRotation);
	//vLocalLook = Vector3::Transform(vLocalLook, matRotation);
	//vLocalLook.Normalize();
	//vLocalUp.Normalize();
	//float fHeight = m_pTransform->m_vPos.y;
	//m_pTransform->m_vPos = m_pTransform->m_vTarget - vLocalLook * m_fDistance;
	//m_pTransform->m_vPos.y = fHeight;
}


bool myCamera::Init()
{
	return true;
}
bool myCamera::Frame()
{
	m_pTransform->LookAt(m_pTransform->m_vTarget);
	m_pTransform->UpdateVector();
	return true;
}
bool myCamera::Release()
{
	m_Frustum.Release();
	return false;
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

bool myCamera::FrameFrustum()
{
	Matrix matInvViewProj = m_pTransform->m_matView * m_pTransform->m_matProj;
	matInvViewProj = matInvViewProj.Invert();

	//뷰행렬과 투영행렬의 역행렬을 만든다

	m_Frustum.Frame(matInvViewProj);
	return true;
}
bool myCamera::DrawFrustum(ID3D11DeviceContext*	pd3dContext, Matrix * pmatView, Matrix * pmatProj)
{
	m_Frustum.m_FrustumObj.m_pTransform->SetMatrix(NULL,
		pmatView,
		pmatProj);
	m_Frustum.m_FrustumObj.Render(pd3dContext);
	return true;
}
myCamera::myCamera()
{
	m_bDrag = false;
	m_fWheelDelta = 0;
}
myCamera::~myCamera()
{
}