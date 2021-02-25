#include "Sample.h"
GAMERUN;

bool Sample::Init()
{
	HRESULT hr = NULL;


	m_Map = new myHeightMap;
	g_ObjMgr.CreateObjComponent(L"Map", m_Map);
	m_Map->CreateHeightMap(m_pd3dContext, L"../../data/castle_height.bmp");


	myMapDesc desc;
	desc.iNumCols = m_Map->m_iNumCols;
	desc.iNumRows = m_Map->m_iNumRows;
	desc.fCellDistance = 1;
	desc.szTexFile = L"../../data/castle.jpg";
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


	myModelViewCamera* pModelCamera = new myModelViewCamera;
	g_CamMgr.CreateCameraObj(m_pd3dContext, L"ModelCamera", pModelCamera);
	//프러스텀 달기
	pModelCamera->CreateViewMatrix({ 0,5,-5 }, { 0,0,0 });
	float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	pModelCamera->CreateProjMatrix(1, 1000, PI2D, fAspect);

	m_TopCamera = new myCamera;
	g_CamMgr.CreateCameraObj(m_pd3dContext, L"TopCamera", m_TopCamera);

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
	//최종적으로는 모든 함수는 각자의 Render에서 돌아가게끔 해야함
	//그걸 호출하는건 obj매니저에 있는 메인gameobject

	//IA에 그려줄 타입 설정
	m_pd3dContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//레스터라이저 스테이트 세팅
	m_pd3dContext->RSSetState(myDxState::m_pRS);
	//픽셀 섀이더에 샘플러 세팅(보간법)
	m_pd3dContext->PSSetSamplers(0, 1, &myDxState::m_pWrapLinear);
	//뎁스 스탠실 스테이트 세팅(깊이값 버퍼)
	m_pd3dContext->OMSetDepthStencilState(myDxState::m_pDSS, 0);


	////// CULLING
	//std::vector<DWORD> visibleIB;
	//visibleIB.resize(m_Map->m_IndexList.size());
	//m_Map->m_iNumFaces = 0;
	//for (int iFace = 0; iFace < m_Map->m_IndexList.size() / 3; iFace++)
	//{
	//	//맵의 각 페이스별로 인덱스 버퍼를 가져온다
	//	int a = m_Map->m_IndexList[iFace * 3 + 0];
	//	int b = m_Map->m_IndexList[iFace * 3 + 1];
	//	int c = m_Map->m_IndexList[iFace * 3 + 2];
	//	//visibleIB.push_back(a);
	//	//visibleIB.push_back(b);
	//	//visibleIB.push_back(c);
	//	//continue;
	//	//그 인덱스 정보로 버텍스 정보를 가져온다
	//	Vector3 v[3];
	//	v[0] = m_Map->m_VertexList[a].p;
	//	v[1] = m_Map->m_VertexList[b].p;
	//	v[2] = m_Map->m_VertexList[c].p;
	//	
	//	//프러스텀 컬링 판별을 해서 그려줄 페이스를 추려서 그려줄 버텍스의 인덱스 버퍼를 누적시킨다
	//	//myModelViewCamera* pCamera = (myModelViewCamera*)m_pMainCamera;
	//	for (int iV = 0; iV < 3; iV++)
	//	{
	//		BOOL bVisiable = g_CamMgr.m_pMainCamera->m_Frustum.ClassifyPoint(v[iV]);
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
	//	//맵의 컬링된 페이스 정보를 세팅한다
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
		m_Map->m_pTransform->SetMatrix(NULL,
			&m_TopCamera->m_pTransform->m_matView,
			&m_TopCamera->m_pTransform->m_matProj);
		m_Map->Render();

		Matrix matWorld;
		matWorld._41 = m_TopCamera->m_pTransform->m_vPos.x;
		matWorld._42 = m_TopCamera->m_pTransform->m_vPos.y;
		matWorld._43 = m_TopCamera->m_pTransform->m_vPos.z;

		m_Box->m_pTransform->SetMatrix(&m_Box->m_pTransform->m_matWorld,
			&m_TopCamera->m_pTransform->m_matView,
			&m_TopCamera->m_pTransform->m_matProj);
		m_Box->Render();
		m_MiniMap->End();
	}

	m_Map->m_pTransform->SetMatrix(NULL,
		&g_pMainCamTransform->m_matView,
		&g_pMainCamTransform->m_matProj);
	m_Map->Render();

	//그리기
	//m_Box.SetMatrix(&m_matBoxWorld,
	//	&m_pMainCamera->m_matView,
	//	&m_pMainCamera->m_matProj);


	m_Box->m_pTransform->SetMatrix(&m_Box->m_pTransform->m_matWorld,
		&g_pMainCamTransform->m_matView, &g_pMainCamTransform->m_matProj);
	m_Box->Render();


	m_MiniMap->m_pTransform->SetMatrix(NULL,
		NULL, //&m_pMainCamera->m_matView,
		NULL); //&m_pMainCamera->m_matProj);
	m_MiniMap->Render();

	return true;
}

bool Sample::Release()
{
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
	if (g_CamMgr.m_pMainCamera == nullptr) return -1;
	int iRet = g_CamMgr.m_pMainCamera->WndProc(hWnd, message, wParam, lParam);
	return -1;
}

