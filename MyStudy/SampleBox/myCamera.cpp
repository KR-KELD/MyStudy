#include "myCamera.h"
#include "myInput.h"
bool		myCamera::CreateViewMatrix(
	myVector3 p,
	myVector3 t,
	myVector3 u)
{
	m_vCameraPos = p;
	m_vCameraTarget = t;
	m_fDistance = (m_vCameraPos - m_vCameraTarget).Length();
	m_matView.CreateViewLook(p, t, u);
	UpdateVector();
	return true;
}
bool		myCamera::CreateProjMatrix(
	float fN,
	float fF,
	float fFov,
	float fAspect)
{
	m_matProj.PerspectiveFovLH(fN, fF, fFov, fAspect);
	return true;
}
void myCamera::SetPos(myVector3 p)
{
	m_vCameraPos = p;
}
void myCamera::SetTarget(myVector3 p)
{
	m_vCameraTarget = p;
}
void myCamera::Update(myVector4 data)
{
	myMatrix matRotation;
	matRotation.YRotate(data.y);
	myVector3 vLocalUp = { 0,1,0 };
	myVector3 vLocalLook = { 0,0,1 };
	vLocalUp = matRotation * vLocalUp;
	vLocalLook = matRotation * vLocalLook;
	vLocalLook.Normal();
	vLocalUp.Normal();
	float fHeight = m_vCameraPos.y;
	m_vCameraPos = m_vCameraTarget - vLocalLook * m_fDistance;
	m_vCameraPos.y = fHeight;
}
void myCamera::FrontMovement(float fDir)
{
	myVector3 vOffset = m_vLook * g_fSecondPerFrame * m_pSpeed * fDir;
	m_vCameraPos += vOffset;
}
void myCamera::RightMovement(float fDir)
{
	myVector3 vMove = m_vRight * g_fSecondPerFrame * m_pSpeed * fDir;
	m_vCameraPos += vMove;
}
void myCamera::UpMovement(float fDir)
{
	myVector3 vMove = m_vUp * g_fSecondPerFrame * m_pSpeed * fDir;
	m_vCameraPos += vMove;
}
void myCamera::FrontBase(float fDir)
{
	myVector3 vSide = { 0,0,1 };
	myVector3 vMove = vSide * g_fSecondPerFrame * m_pSpeed * fDir;
	m_vCameraPos += vMove;
	m_vCameraTarget += m_vLook * m_pSpeed;
}
void myCamera::RightBase(float fDir)
{
	myVector3 vSide = { 1,0,0 };
	myVector3 vMove = vSide * g_fSecondPerFrame * m_pSpeed * fDir;
	m_vCameraPos += vMove;
	m_vCameraTarget += m_vLook * m_pSpeed;
}
void myCamera::UpBase(float fDir)
{
	myVector3 vUp = { 0,1,0 };
	myVector3 vMove = vUp * g_fSecondPerFrame * m_pSpeed * fDir;
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
	myVector3 vUp = { 0,1,0 };
	m_matView.CreateViewLook(
		m_vCameraPos,
		m_vCameraTarget,
		vUp);
	UpdateVector();
	return true;
}
myCamera::myCamera()
{
	m_pSpeed = 30.0f;
}
myCamera::~myCamera()
{

}