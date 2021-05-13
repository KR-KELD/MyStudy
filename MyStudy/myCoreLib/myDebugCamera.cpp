#include "myDebugCamera.h"
#include "myInput.h"
DECLARE_COMPONENT(myDebugCamera);
void myDebugCamera::Update(Vector4 data)
{
	Matrix matRoation;
	matRoation = Matrix::CreateFromYawPitchRoll(
		data.y, data.x, data.z);
	//D3DXMatrixAffineTransformation
	// matRotation = quaternion * pos * scale;
	//m_pTransform->m_vPos += m_pTransform->m_vLook * data.w;
	m_pTransform->m_vPos += m_pTransform->m_vLook * m_fWheelDelta;
	//TBASIS_CORE_LIB::OutputDebug("%10.4f\n", m_fWheelDelta);
	matRoation._41 = m_pTransform->m_vPos.x;
	matRoation._42 = m_pTransform->m_vPos.y;
	matRoation._43 = m_pTransform->m_vPos.z;

	m_pTransform->m_matView = matRoation.Invert();
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
	//MoveCamera();
	POINT point;
	GetCursorPos(&point);
	HWND hWnd = WindowFromPoint(point);
	ScreenToClient(g_hWnd, &point);

	if (hWnd == g_hWnd && g_Input.GetKey(VK_RBUTTON))
	{
		float fAngleX = (point.x - m_ptPrePosition.x);
		float fAngleY = (point.y - m_ptPrePosition.y);
		m_pTransform->m_vRot.y += XMConvertToRadians(fAngleX / 2.0f);
		m_pTransform->m_vRot.x += XMConvertToRadians(fAngleY / 2.0f);
		//TBASIS_CORE_LIB::OutputDebug("%10.4f*10.4f\n",fAngleX,fAngleY);
	}
	m_ptPrePosition = point;

	Update(m_pTransform->m_vRot);
	m_pTransform->UpdateVector();
	return true;
}

myDebugCamera::myDebugCamera()
{
}

myDebugCamera::~myDebugCamera()
{

}
