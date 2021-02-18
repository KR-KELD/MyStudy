#include "myDebugCamera.h"
#include "myInput.h"
void myDebugCamera::Update(Vector4 data)
{
	Matrix matRoation;
	matRoation = Matrix::CreateFromYawPitchRoll(
		data.y, data.x, data.z);
	//D3DXMatrixAffineTransformation
	// matRotation = quaternion * pos * scale;
	//m_vCameraPos += m_vLook * data.w;
	m_vCameraPos += m_vLook * m_fWheelDelta;
	//TBASIS_CORE_LIB::OutputDebug("%10.4f\n", m_fWheelDelta);
	matRoation._41 = m_vCameraPos.x;
	matRoation._42 = m_vCameraPos.y;
	matRoation._43 = m_vCameraPos.z;

	m_matView = matRoation.Invert();
	m_fWheelDelta = 0;
}

bool myDebugCamera::Init()
{
	myCamera::Init();
	m_ptPrePosition = g_Input.GetMouse();
	return false;
}

bool myDebugCamera::Frame()
{
	POINT point;
	GetCursorPos(&point);
	HWND hWnd = WindowFromPoint(point);
	ScreenToClient(g_hWnd, &point);

	if (hWnd == g_hWnd && g_Input.GetKey(VK_LBUTTON))
	{
		float fAngleX = (point.x - m_ptPrePosition.x);
		float fAngleY = (point.y - m_ptPrePosition.y);
		m_vDirValue.y += XMConvertToRadians(fAngleX / 2.0f);
		m_vDirValue.x += XMConvertToRadians(fAngleY / 2.0f);
		//TBASIS_CORE_LIB::OutputDebug("%10.4f*10.4f\n",fAngleX,fAngleY);
	}
	m_ptPrePosition = point;

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
