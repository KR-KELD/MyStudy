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
		m_WorldArcball.OnBegin(iMouseX, iMouseY);
	}break;
	case WM_RBUTTONDOWN:
	{
		m_ViewArcball.OnBegin(iMouseX, iMouseY);
	}break;
	case WM_MOUSEMOVE:
	{
		m_WorldArcball.OnMove(iMouseX, iMouseY);
		m_ViewArcball.OnMove(iMouseX, iMouseY);
	}break;
	case WM_LBUTTONUP:
	{
		m_WorldArcball.OnEnd();
	}break;
	case WM_RBUTTONUP:
	{
		m_ViewArcball.OnEnd();
	}break;
	case WM_MOUSEWHEEL:
	{
		m_fWheelDelta += GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA;
	}break;
	}
	return -1;
}
bool myModelViewCamera::CreateFrustum(ID3D11Device * pd3dDevice, ID3D11DeviceContext * d3dContext)
{
	m_Frustum.Create(pd3dDevice);
	return true;
}
bool myModelViewCamera::PostInit()
{
	//뷰행렬의 역행렬을 
	//질문
	Matrix matInvView = m_matView.Invert();
	m_ViewArcball.m_qNow = Quaternion::CreateFromRotationMatrix(matInvView);
	m_ViewArcball.m_qNow.Normalize();
}
Quaternion myArcball::QuatFromPoints(Vector3 v0, Vector3 v1)
{
	Vector3 vCross;
	float fDot = v0.Dot(v1);
	vCross = v0.Cross(v1);
	return Quaternion(vCross.x, vCross.y, vCross.z, fDot);;
}
Vector3 myArcball::ScreenToVector(float px, float py)
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
void myArcball::OnBegin(int x, int y)
{
	m_bDrag = true;
	m_vDownPt = ScreenToVector(x, y);
	m_qDown = m_qNow;
}
void myArcball::OnMove(int x, int y)
{
	if (m_bDrag)
	{
		m_vCurrentPt = ScreenToVector(x, y);
		m_qNow = m_qDown * QuatFromPoints(m_vDownPt, m_vCurrentPt);
	}
}
void myArcball::OnEnd()
{
	m_bDrag = false;
}

Matrix myArcball::GetRotationMatrix()
{
	return Matrix::CreateFromQuaternion(m_qNow);
}
void myModelViewCamera::Update(Vector4 data)
{

}
bool myModelViewCamera::Frame()
{
	//VIEW
	m_fDistance += m_fWheelDelta;
	Matrix matCameraRotation;
	matCameraRotation = m_ViewArcball.GetRotationMatrix();
	Vector3 vLocalUp = { 0,1,0 };
	Vector3 vLocalLook = { 0,0,1 };
	// TODO V*M
	vLocalUp = Vector3::Transform(vLocalUp, matCameraRotation);
	vLocalLook = Vector3::Transform(vLocalLook, matCameraRotation);
	vLocalLook.Normalize();
	vLocalUp.Normalize();

	m_pTransform->m_vPos = m_pTransform->m_vTarget - vLocalLook * m_fDistance;
	CreateViewMatrix(m_pTransform->m_vPos, m_pTransform->m_vTarget);

	//WORLD
	Matrix mInvView = m_matView.Invert();
	mInvView._41 = 0.0f;
	mInvView._42 = 0.0f;
	mInvView._43 = 0.0f;

	Matrix mModelRotInv = m_mModelLastRot.Invert();

	Matrix mModelRot = m_WorldArcball.GetRotationMatrix();
	m_matWorld = m_matWorld * m_matView * mModelRotInv * mModelRot * mInvView;

	m_matWorld._41 = 0;
	m_matWorld._42 = 0;
	m_matWorld._43 = 0;
	m_mModelLastRot = mModelRot;

	UpdateVector();

	m_fWheelDelta = 0;
	return true;
}
void myModelViewCamera::UpdateVector()
{
	//카메라 위치,방향 갱신
	myCamera::UpdateVector();
}
bool myModelViewCamera::DrawFrustum(ID3D11DeviceContext * pd3dContext, Matrix * pmatView, Matrix * pmatProj)
{
	//매트릭스를 세팅하고 프러스텀 박스 랜더
	m_Frustum.m_FrustumObj.SetMatrix(NULL,
		pmatView,
		pmatProj);
	m_Frustum.m_FrustumObj.Render(pd3dContext);
	return true;
}
bool myModelViewCamera::FrameFrustum(ID3D11DeviceContext * pd3dContext)
{
	//뷰행렬과 투영행렬의 역행렬을 만든다
	Matrix matInvViewProj = m_matView * m_matProj;
	matInvViewProj = matInvViewProj.Invert();

	for (int iVertex = 0; iVertex < 24; iVertex++)
	{
		//ndc공간의 프러스텀 정점들에 뷰*투영행렬의 역행렬을 곱해서 월드 공간 프러스텀으로 만들어준다
		Vector3& v = m_Frustum.m_VertexList[iVertex].p;
		m_Frustum.m_FrustumObj.m_VertexList[iVertex].p = Vector3::Transform(v, matInvViewProj);// *matInvViewProj;
	}
	//변환된 프러스텀의 정점과 정점 버퍼를 세팅해준다
	pd3dContext->UpdateSubresource(
		m_Frustum.m_FrustumObj.m_pVertexBuffer, 0, NULL,
		&m_Frustum.m_FrustumObj.m_VertexList.at(0), 0, 0);

	m_Frustum.Frame();
	return true;
}
myModelViewCamera::myModelViewCamera()
{

}
myModelViewCamera::~myModelViewCamera()
{
	m_Frustum.m_FrustumObj.Release();
}