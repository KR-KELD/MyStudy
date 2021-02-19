#include "Sample.h"
GAMERUN;

bool Sample::Init()
{
	HRESULT hr = NULL;
	m_vDirValue = { 0,0,0,0 };

	Matrix matScale, matRotation;
	matScale = Matrix::CreateScale(100, 100, 100);
	matRotation = Matrix::CreateRotationX(PI2D);
	m_matPlaneWorld = matScale * matRotation;

	m_Box = new myShapeBox;
	g_ObjMgr.CreateComponentInObj(L"Box", m_Box);

	m_Plane = new myShapePlane;
	g_ObjMgr.CreateComponentInObj(L"Plane", m_Plane);

	m_Line = new myShapeLine;
	g_ObjMgr.CreateComponentInObj(L"Line", m_Line);


	if (!m_Box->Create(m_pd3dDevice, L"vs.txt", L"ps.txt",
		L"../../data/bitmap/flametank.bmp"))
	{
		return false;
	}
	if (!m_Plane->Create(m_pd3dDevice, L"vs.txt", L"ps.txt",
		L"../../data/bitmap/flametank.bmp"))
	{
		return false;
	}
	if (!m_Line->Create(m_pd3dDevice, L"vs.txt", L"ps.txt",
		L"../../data/bitmap/flametank.bmp"))
	{
		return false;
	}
	//m_ModelCamera.CreateViewMatrix({ 0,5,-5 }, { 0,0,0 });
	//float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	//m_ModelCamera.CreateProjMatrix(1, 1000, PI2D, fAspect);
	//m_ModelCamera.Init();
	//m_pMainCamera = &m_ModelCamera;

	m_Map = new myMap;
	g_ObjMgr.CreateComponentInObj(L"Map", m_Map);

	myMapDesc desc;
	desc.iNumCols = 513;
	desc.iNumRows = 513;
	desc.fCellDistance = 1;
	desc.szTexFile = L"../../data/tileA.jpg";
	desc.szVS = L"VS.txt";
	desc.szPS = L"PS.txt";
	m_Map->CreateMap(m_pd3dDevice, desc);
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

	return true;
}

bool Sample::Render()
{
	//IA�� �׷��� Ÿ�� ����
	m_pd3dContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//�����Ͷ����� ������Ʈ ����
	m_pd3dContext->RSSetState(myDxState::m_pRS);
	//�ȼ� ���̴��� ���÷� ����(������)
	m_pd3dContext->PSSetSamplers(0, 1, &myDxState::m_pWrapLinear);
	//���� ���Ľ� ������Ʈ ����(���̰� ����)
	m_pd3dContext->OMSetDepthStencilState(myDxState::m_pDSS, 0);
	//�׸���
	//m_Box.SetMatrix(&m_matBoxWorld,
	//	&m_pMainCamera->m_matView,
	//	&m_pMainCamera->m_matProj);
	m_Box->SetMatrix(&m_pMainCamera->m_matWorld,
		&m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	m_Box->Render(m_pd3dContext);
	Matrix matShadow;
	Vector4 vPlane = Vector4(0, 1, 0, -0.1f);
	Vector3 vLightDir = Vector3(-10, 10, 0);
	matShadow = Matrix::CreateShadow(vLightDir, vPlane);
	matShadow = m_matBoxWorld * matShadow;
	m_Box->SetMatrix(&matShadow, &m_pMainCamera->m_matView,
		&m_pMainCamera->m_matProj);
	m_Box->Render(m_pd3dContext);

	m_Plane->SetMatrix(&m_matPlaneWorld,
		&m_pMainCamera->m_matView,
		&m_pMainCamera->m_matProj);
	//m_Plane->Render(m_pd3dContext);

	m_Map->SetMatrix(NULL,
		&m_pMainCamera->m_matView,
		&m_pMainCamera->m_matProj);
	m_Map->Render(m_pd3dContext);

	m_Line->SetMatrix(NULL, &m_pMainCamera->m_matView,
		&m_pMainCamera->m_matProj);
	m_Line->Draw(m_pd3dContext,
		Vector3(0, 0, 0), Vector3(100, 0, 0), Vector4(1, 0, 0, 1));
	m_Line->Draw(m_pd3dContext,
		Vector3(0, 0, 0), Vector3(0, 100, 0), Vector4(0, 1, 0, 1));
	m_Line->Draw(m_pd3dContext,
		Vector3(0, 0, 0), Vector3(0, 0, 100), Vector4(0, 0, 1, 1));

	return true;
}

bool Sample::Release()
{
	//myCore::Release();
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
	if (m_pMainCamera == nullptr) return -1;
	int iRet = m_pMainCamera->WndProc(hWnd, message, wParam, lParam);
	return -1;
}

