#include "myModelViewCamera.h"
int myModelViewCamera::WndProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
	int iMouseX = LOWORD(lParam);
	int iMouseY = HIWORD(lParam);
	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		m_bDrag = true;
		OnBegin(iMouseX, iMouseY);
	}break;
	case WM_MOUSEMOVE:
	{
		if (m_bDrag)
		{
			OnMove(iMouseX, iMouseY);
		}
	}break;
	case WM_LBUTTONUP:
	{
		m_bDrag = false;
		OnEnd();
	}break;
	case WM_MOUSEWHEEL:
	{
		m_fWheelDelta += GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA;
	}break;
	}
	return -1;
}
Quaternion myModelViewCamera::QuatFromPoints(Vector3 v0, Vector3 v1)
{
	Vector3 vCross;
	float fDot = v0.Dot(v1);
	vCross = v0.Cross(v1);
	return Quaternion(vCross.x, vCross.y, vCross.z, fDot);;
}
Vector3 myModelViewCamera::ScreenToVector(float px, float py)
{
	float x = -((px / (float)g_rtClient.right) * 2.0f - 1.0f);
	float y = (py / (float)g_rtClient.bottom) * 2.0f - 1.0f;
	float z = 0.0f;
	float fDist = x * x + y * y;
	if (fDist > 1.0f)
	{
		float fScale = (1.0f / sqrtf(fDist));
		x *= fScale;
		y *= fScale;
	}
	else
	{
		z = sqrtf(1.0f - fDist);
	}
	return Vector3(x, y, z);
}
void myModelViewCamera::OnBegin(int x, int y)
{
	m_vDownPt = ScreenToVector(x, y);
	m_qDown = m_qNow;
}
void myModelViewCamera::OnMove(int x, int y)
{
	m_vCurrentPt = ScreenToVector(x, y);
	m_qDown = m_qNow * QuatFromPoints(m_vDownPt, m_vCurrentPt);
}
void myModelViewCamera::OnEnd()
{
}
Matrix myModelViewCamera::GetRotationMatrix()
{
	return Matrix::CreateFromQuaternion(m_qDown);
}
void myModelViewCamera::Update(Vector4 data)
{
	Matrix mInvView = m_matView.Invert();
	mInvView._41 = 0.0f;
	mInvView._42 = 0.0f;
	mInvView._43 = 0.0f;

	Matrix mModelRotInv = m_mModelLastRot.Invert();

	Matrix mModelRot = GetRotationMatrix();
	m_matWorld = m_matWorld * m_matView * mModelRotInv * mModelRot * mInvView;

	m_mModelLastRot = mModelRot;
	//Matrix matRotation;
	//matRotation = Matrix::CreateRotationY(data.y);
	//Vector3 vLocalUp = { 0,1,0 };
	//Vector3 vLocalLook = { 0,0,1 };
	//// TODO V*M
	//vLocalUp = Vector3::Transform(vLocalUp, matRotation);
	//vLocalLook = Vector3::Transform(vLocalLook, matRotation);
	//vLocalLook.Normalize();
	//vLocalUp.Normalize();
	//float fHeight = m_vCameraPos.y;
	//m_vCameraPos = m_vCameraTarget - vLocalLook * m_fDistance;
	//m_vCameraPos.y = fHeight;
}
bool myModelViewCamera::Frame()
{
	Matrix mInvView = m_matView.Invert();
	mInvView._41 = 0.0f;
	mInvView._42 = 0.0f;
	mInvView._43 = 0.0f;

	Matrix mModelRotInv = m_mModelLastRot.Invert();

	Matrix mModelRot = GetRotationMatrix();
	m_matWorld = m_matWorld * m_matView * mModelRotInv * mModelRot * mInvView;

	m_matWorld._41 = 0;
	m_matWorld._42 = 0;
	m_matWorld._43 = 0;
	m_mModelLastRot = mModelRot;

	UpdateVector();
	return true;
}
myModelViewCamera::myModelViewCamera()
{
	m_qNow = Quaternion::Identity;
	m_qDown = Quaternion::Identity;
}
myModelViewCamera::~myModelViewCamera()
{

}