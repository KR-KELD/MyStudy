#include "Sample.h"
GAMERUN;

bool Sample::Init()
{
	HRESULT hr = NULL;


	m_Map = new myHeightMap;
	g_ObjMgr.CreateObjComponent(L"Map", m_Map);
	m_Map->CreateHeightMap(m_pd3dContext, L"../../data/map/Map512.png");


	myMapDesc desc;
	desc.iNumCols = m_Map->m_iNumCols;
	desc.iNumRows = m_Map->m_iNumRows;
	desc.fCellDistance = 1;
	desc.szTexFile = L"../../data/map/Map512Color.png";
	desc.szVS = L"VS.txt";
	desc.szPS = L"PS.txt";
	m_Map->CreateMap(m_pd3dContext, desc);

	m_MiniMap = new myMiniMap;
	g_ObjMgr.CreateObjComponent(L"MiniMap", m_MiniMap);

	m_MiniMap->Create(m_pd3dContext, L"vs.txt", L"ps.txt",
		L"../../data/bitmap/map.bmp");

	Matrix matScale, matRotation;
	matScale = Matrix::CreateScale(100, 100, 100);
	matRotation = Matrix::CreateRotationX(PI2D);
	m_matPlaneWorld = matScale * matRotation;

	m_Box = new myShapeBox;
	g_ObjMgr.CreateObjComponent(L"Box", m_Box);

	if (!m_Box->Create(m_pd3dContext, L"vs.txt", L"ps.txt",
		L"../../data/bitmap/intro.bmp"))
	{
		return false;
	}

	myModelViewCamera* m_ModelCamera;
	m_ModelCamera = new myModelViewCamera;
	m_ModelCameraObj = myGameObject::CreateGameObject(L"ModelCamera");
	m_ModelCameraObj->InsertComponent(m_ModelCamera);
	m_ModelCamera->Init();
	m_ModelCamera->CreateFrustum(m_pd3dContext);
	//�������� �ޱ�
	m_ModelCamera->CreateViewMatrix({ 0,5,-5 }, { 0,0,0 });
	float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	m_ModelCamera->CreateProjMatrix(1, 1000, PI2D, fAspect);
	//m_pMainCamera = m_ModelCamera;

	m_TopCamera = new myCamera;
	g_ObjMgr.CreateObjComponent(L"TopCamera", m_TopCamera);
	m_TopCamera->CreateFrustum(m_pd3dContext);

	m_TopCamera->CreateViewMatrix({ 0,30.0f,-0.1f }, { 0,0,0 });
	fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	m_TopCamera->CreateOrthographic(500, 500, 1.0f, 1000);




	return true;
}

bool Sample::Frame()
{
	//Matrix matScale;
	//Matrix matRotation;

	//matScale = Matrix::CreateScale(1, 1, 1);
	//matRotation = Matrix::CreateRotationY(g_fGameTimer);
	//m_matBoxWorld = matScale * matRotation;
	//m_matBoxWorld._42 = 3.0f;

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
	//���������δ� ��� �Լ��� ������ Render���� ���ư��Բ� �ؾ���
	//�װ� ȣ���ϴ°� obj�Ŵ����� �ִ� ����gameobject

	//IA�� �׷��� Ÿ�� ����
	m_pd3dContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//�����Ͷ����� ������Ʈ ����
	m_pd3dContext->RSSetState(myDxState::m_pRS);
	//�ȼ� ���̴��� ���÷� ����(������)
	m_pd3dContext->PSSetSamplers(0, 1, &myDxState::m_pWrapLinear);
	//���� ���Ľ� ������Ʈ ����(���̰� ����)
	m_pd3dContext->OMSetDepthStencilState(myDxState::m_pDSS, 0);


	//// CULLING
	//std::vector<DWORD> visibleIB;
	//visibleIB.resize(m_Map->m_IndexList.size());
	//m_Map->m_iNumFaces = 0;
	//for (int iFace = 0; iFace < m_Map->m_IndexList.size() / 3; iFace++)
	//{
	//	//���� �� ���̽����� �ε��� ���۸� �����´�
	//	int a = m_Map->m_IndexList[iFace * 3 + 0];
	//	int b = m_Map->m_IndexList[iFace * 3 + 1];
	//	int c = m_Map->m_IndexList[iFace * 3 + 2];
	//	//visibleIB.push_back(a);
	//	//visibleIB.push_back(b);
	//	//visibleIB.push_back(c);
	//	//continue;
	//	//�� �ε��� ������ ���ؽ� ������ �����´�
	//	Vector3 v[3];
	//	v[0] = m_Map->m_VertexList[a].p;
	//	v[1] = m_Map->m_VertexList[b].p;
	//	v[2] = m_Map->m_VertexList[c].p;
	//	
	//	//�������� �ø� �Ǻ��� �ؼ� �׷��� ���̽��� �߷��� �׷��� ���ؽ��� �ε��� ���۸� ������Ų��
	//	//myModelViewCamera* pCamera = (myModelViewCamera*)m_pMainCamera;
	//	for (int iV = 0; iV < 3; iV++)
	//	{
	//		BOOL bVisiable = m_pMainCamera->m_Frustum.ClassifyPoint(v[iV]);
	//		if (bVisiable)
	//		{
	//			visibleIB[m_Map->m_iNumFaces * 3 + 0] = a;
	//			visibleIB[m_Map->m_iNumFaces * 3 + 1] = b;
	//			visibleIB[m_Map->m_iNumFaces * 3 + 2] = c;
	//			m_Map->m_iNumFaces++;
	//			break;
	//		}
	//	}
	//}
	//if (visibleIB.size() != 0)
	//{
	//	//���� �ø��� ���̽� ������ �����Ѵ�
	//	m_Map->m_iNumFaces = visibleIB.size() / 3;
	//	m_pd3dContext->UpdateSubresource(
	//		m_Map->m_pIndexBuffer, 0, NULL, &visibleIB.at(0), 0, 0);
	//}
	//else
	//{
	//	m_Map->m_iNumFaces = 0;
	//}

	if (m_MiniMap->Begin())
	{
		m_Map->SetMatrix(NULL,
			&m_TopCamera->m_matView,
			&m_TopCamera->m_matProj);
		m_Map->Render();

		Matrix matWorld;
		matWorld._41 = m_TopCamera->m_pTransform->m_vPos.x;
		matWorld._42 = m_TopCamera->m_pTransform->m_vPos.y;
		matWorld._43 = m_TopCamera->m_pTransform->m_vPos.z;

		m_Box->SetMatrix(&m_Box->m_matWorld,
			&m_TopCamera->m_matView,
			&m_TopCamera->m_matProj);
		m_Box->Render();
		m_MiniMap->End();
	}

	m_Map->SetMatrix(NULL,
		&m_pMainCamera->m_matView,
		&m_pMainCamera->m_matProj);
	m_Map->Render();

	//�׸���
	//m_Box.SetMatrix(&m_matBoxWorld,
	//	&m_pMainCamera->m_matView,
	//	&m_pMainCamera->m_matProj);


	m_Box->SetMatrix(&m_Box->m_matWorld,
		&m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	m_Box->Render();


	m_MiniMap->SetMatrix(NULL,
		NULL, //&m_pMainCamera->m_matView,
		NULL); //&m_pMainCamera->m_matProj);
	m_MiniMap->Render();

	return true;
}

bool Sample::Release()
{
	m_ModelCameraObj->Release();
	delete m_ModelCameraObj;
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

