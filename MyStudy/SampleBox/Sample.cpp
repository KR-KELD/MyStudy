#include "Sample.h"
GAMERUN;
bool Sample::Init()
{
	HRESULT hr = NULL;
	m_vDirValue = { 0,0,0,0 };
	m_Camera.CreateViewMatrix({ 0,10,-10 }, { 0,0,0 });
	float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	m_Camera.CreateProjMatrix(1, 1000, PI2D, fAspect);

	Matrix matScale, matRotation;
	matScale = Matrix::CreateScale(100, 100, 100);
	matRotation = Matrix::CreateRotationX(PI2D);
	m_matPlaneWorld = matScale * matRotation;

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
	Matrix matScale;
	Matrix matRotation;

	matScale = Matrix::CreateScale(1, 1, 1);
	matRotation = Matrix::CreateRotationY(g_fGameTimer);
	m_matBoxWorld = matScale * matRotation;
	m_matBoxWorld._42 = 3.0f;

	if (g_Input.GetKey('0') == KEY_PUSH)
	{
		myDxState::m_FillMode = D3D11_FILL_WIREFRAME;
		myDxState::SetRasterizerState(m_pd3dDevice);
	}
	if (g_Input.GetKey('9') == KEY_PUSH)
	{
		myDxState::m_FillMode = D3D11_FILL_SOLID;
		myDxState::SetRasterizerState(m_pd3dDevice);
	}
	if (g_Input.GetKey('8') == KEY_PUSH)
	{
		myDxState::m_CullMode = D3D11_CULL_BACK;
		myDxState::SetRasterizerState(m_pd3dDevice);
	}
	if (g_Input.GetKey('7') == KEY_PUSH)
	{
		myDxState::m_CullMode = D3D11_CULL_FRONT;
		myDxState::SetRasterizerState(m_pd3dDevice);
	}

	if (g_Input.GetKey('W') == KEY_HOLD)
	{
		m_Camera.FrontMovement(1.0f);
	}
	if (g_Input.GetKey('S') == KEY_HOLD)
	{
		m_Camera.FrontMovement(-1.0f);
	}
	if (g_Input.GetKey('A') == KEY_HOLD)
	{
		m_Camera.RightMovement(1.0f);
	}
	if (g_Input.GetKey('D') == KEY_HOLD)
	{
		m_Camera.RightMovement(-1.0f);
	}
	if (g_Input.GetKey('Q') == KEY_HOLD)
	{
		m_Camera.UpMovement(1.0f);
	}
	if (g_Input.GetKey('E') == KEY_HOLD)
	{
		m_Camera.UpMovement(-1.0f);
	}
	//m_Camera.SetPos(m_Camera.m_vCameraPos);	
	m_Camera.Frame();

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
	m_Box.SetMatrix(&m_matBoxWorld,
		&m_Camera.m_matView,
		&m_Camera.m_matProj);
	m_Box.Render(m_pd3dContext);
	Matrix matShadow;
	Vector4 PLANE = Vector4(0, 1, 0, -0.1f);
	Vector4 LIGHT = Vector4(-10, 10, 0, 1);
	matShadow = CreateMatrixShadow(&PLANE, &LIGHT);
	matShadow = m_matBoxWorld * matShadow;
	m_Box.SetMatrix(&matShadow, &m_Camera.m_matView,
		&m_Camera.m_matProj);
	m_Box.Render(m_pd3dContext);

	m_Plane.SetMatrix(&m_matPlaneWorld,
		&m_Camera.m_matView,
		&m_Camera.m_matProj);
	m_Plane.Render(m_pd3dContext);
	m_Line.SetMatrix(NULL, &m_Camera.m_matView,
		&m_Camera.m_matProj);
	m_Line.Draw(m_pd3dContext,
		Vector3(0, 0, 0), Vector3(100, 0, 0), Vector4(1, 0, 0, 1));
	m_Line.Draw(m_pd3dContext,
		Vector3(0, 0, 0), Vector3(0, 100, 0), Vector4(0, 1, 0, 1));
	m_Line.Draw(m_pd3dContext,
		Vector3(0, 0, 0), Vector3(0, 0, 100), Vector4(0, 0, 1, 1));

	return true;
}

bool Sample::Release()
{
	m_Box.Relase();
	m_Plane.Relase();
	m_Line.Relase();
	return true;
}

Matrix Sample::CreateMatrixShadow(Vector4 * pPlane, Vector4 * pLight)
{
	Matrix mat;
	Vector4 plane, light;
	pPlane->Normalize();
	plane.x = pPlane->x * -1.0f;
	plane.y = pPlane->y * -1.0f;
	plane.z = pPlane->z * -1.0f;
	plane.w = pPlane->w * -1.0f;
	light = *pLight;// * -1.0f;
	float D = -(plane.Dot(light));
	mat._11 = plane.x * light.x + D;	mat._12 = plane.x * light.y;	mat._13 = plane.x * light.z;	mat._14 = plane.x * light.w;
	mat._21 = plane.y * light.x;	mat._22 = plane.y * light.y + D;	mat._23 = plane.y * light.z;	mat._24 = plane.y * light.w;
	mat._31 = plane.z * light.x;	mat._32 = plane.z * light.y;	mat._33 = plane.z * light.z + D;	mat._34 = plane.z * light.w;
	mat._41 = plane.w * light.x;	mat._42 = plane.w * light.y;	mat._43 = plane.w * light.z;	mat._44 = plane.w * light.w + D;
	return mat;
}

LRESULT Sample::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int iRet = m_Camera.WndProc(hWnd, message, wParam, lParam);
	if (iRet >= 0)
	{
		return iRet;
	}
	return -1;
}

