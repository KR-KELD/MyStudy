#include "myDebugCamera.h"

void myDebugCamera::Update(Vector4 data)
{
	Matrix matRoation;
	matRoation = Matrix::CreateFromYawPitchRoll(
		data.y, data.x, data.z);
	//D3DXMatrixAffineTransformation
	// matRotation = quaternion * pos * scale;
	m_vCameraPos += m_vLook * data.w;
	matRoation._41 = m_vCameraPos.x;
	matRoation._42 = m_vCameraPos.y;
	matRoation._43 = m_vCameraPos.z;

	m_matView = matRoation.Invert();
}

bool myDebugCamera::Frame()
{
	m_vDirValue.y -= m_rtOffset.x * g_fSecondPerFrame * 0.1f;
	m_vDirValue.x -= m_rtOffset.y * g_fSecondPerFrame* 0.1f;
	Update(m_vDirValue);
	UpdateVector();
	return true;
}

myDebugCamera::myDebugCamera()
{
	m_vDirValue.x = 0.0f;
	m_vDirValue.y = 0.0f;
	m_vDirValue.z = 0.0f;
	m_vDirValue.w = 0.0f;
}

myDebugCamera::~myDebugCamera()
{
}
