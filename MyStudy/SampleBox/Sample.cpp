#include "Sample.h"
GAMERUN;
bool Sample::Init()
{
	HRESULT hr = NULL;
	m_vCameraPos = { 0,0,-5 };
	m_vCameraTarget = { 0,0,0 };
	//카메라
	m_matWorld.Identity();
	myVector3 p = m_vCameraPos;
	myVector3 t = m_vCameraTarget;
	myVector3 u = { 0,1,0 };
	m_matView.CreateViewLook(p, t, u);
	float fN = 1;
	float fF = 1000;
	float fFov = PI / 2.0f;
	float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	m_matProj.PerspectiveFovLH(fN, fF, fFov, fAspect);

	if (!m_Box.Create(m_pd3dDevice, L"vs.txt", L"ps.txt",
		L"../../data/bitmap/flametank.bmp"))
	{
		return false;
	}
	if (!m_Plane.Create(m_pd3dDevice, L"vs.txt", L"ps.txt",
		L"../../data/bitmap/flametank.bmp"))
	{
		return false;
	}
	if (!m_Line.Create(m_pd3dDevice, L"vs.txt", L"ps.txt",
		L"../../data/bitmap/flametank.bmp"))
	{
		return false;
	}

	return true;
}

bool Sample::Frame()
{
	myMatrix matScale;
	myMatrix matRotation;
	//matScale.Scale(2, 2, 2);
	//matRotation.YRotate(g_fGameTimer);
	//m_matWorld = matScale * matRotation;
	myMatrix RotY;
	myMatrix RotX;
	if (g_Input.GetKey('A') == KEY_HOLD)
	{
		m_CameraAngleY += 0.001f;
	}

	if (g_Input.GetKey('D') == KEY_HOLD)
	{
		m_CameraAngleY -= 0.001f;
	}
	if (g_Input.GetKey('W') == KEY_HOLD)
	{
		m_CameraAngleX -= 0.001f;
	}
	if (g_Input.GetKey('S') == KEY_HOLD)
	{
		m_CameraAngleX += 0.001f;
	}
	RotY._11 = cosf(m_CameraAngleY);
	RotY._13 = sinf(m_CameraAngleY);
	RotY._31 = -sinf(m_CameraAngleY);
	RotY._33 = cosf(m_CameraAngleY);
	RotX._22 = cosf(m_CameraAngleX);
	RotX._23 = -sinf(m_CameraAngleX);
	RotX._32 = sinf(m_CameraAngleX);
	RotX._33 = cosf(m_CameraAngleX);

	myMatrix Rot = RotX * RotY;
	myVector3 CameraPos = Rot * m_vCameraPos;
	myVector3 u = { 0,1,0 };
	m_matView.CreateViewLook(CameraPos, m_vCameraTarget, u);

	return true;
}

bool Sample::Render()
{
	//IA에 그려줄 타입 설정
	m_pd3dContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//m_pd3dContext->Draw(m_VertexList.size(), 0);
	//레스터라이저 스테이트 세팅
	m_pd3dContext->RSSetState(myDxState::m_pRS);
	//픽셀 섀이더에 샘플러 세팅(보간법)
	m_pd3dContext->PSSetSamplers(0, 1, &myDxState::m_pWrapLinear);
	//뎁스 스탠실 스테이트 세팅(깊이값 버퍼)
	m_pd3dContext->OMSetDepthStencilState(myDxState::m_pDSS, 0);
	//그리기
	m_Box.SetMatrix(&m_matBoxWorld, &m_matView, &m_matProj);
	m_Box.Render(m_pd3dContext);
	m_matPlaneWorld.Translation(0.0f, -3.0f, 0.0f);
	m_Plane.SetMatrix(&m_matPlaneWorld, &m_matView, &m_matProj);
	m_Plane.Render(m_pd3dContext);

	m_Line.SetMatrix(NULL, &m_matView, &m_matProj);
	m_Line.Draw(m_pd3dContext,
		myVector3(0, 0, 0), myVector3(100, 0, 0), myVector4(1, 0, 0, 1));
	m_Line.Draw(m_pd3dContext,
		myVector3(0, 0, 0), myVector3(0, 100, 0), myVector4(0, 1, 0, 1));
	m_Line.Draw(m_pd3dContext,
		myVector3(0, 0, 0), myVector3(0, 0, 100), myVector4(0, 0, 1, 1));

	return true;
}

bool Sample::Release()
{
	m_Box.Relase();
	m_Plane.Relase();
	m_Line.Relase();
	return true;
}

