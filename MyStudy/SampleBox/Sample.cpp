#include "Sample.h"
#define NUM_OBJECTS 10
GAMERUN;

bool Sample::Init()
{
	HRESULT hr = NULL;


	m_Map = new myHeightMap;
	g_ObjMgr.CreateObjComponent(L"Map", m_Map);
	m_Map->CreateHeightMap(L"../../data/castle_height.bmp");


	myMapDesc desc;
	desc.iNumCols = m_Map->m_iNumCols;
	desc.iNumRows = m_Map->m_iNumRows;
	desc.fCellDistance = 1;
	desc.fScaleHeight = 10.0f;
	desc.szTexFile = L"../../data/castle.jpg";
	desc.szVS = L"VS.txt";
	desc.szPS = L"PS.txt";
	m_Map->CreateMap(desc);
	//m_Map->CalNormal();

	m_QuadTree.Build(129, 129);


	m_MiniMap = new myMiniMap;
	//m_MiniObj = myGameObject::CreateGameObject(L"MiniMap");
	//m_MiniObj->InsertComponent(m_MiniMap);
	g_ObjMgr.CreateObjComponent(L"MiniMap", m_MiniMap);

	m_MiniMap->Create(L"vs.txt", L"ps.txt",
		L"../../data/castle.jpg");

	m_Box = new myShapeBox;
	g_ObjMgr.CreateObjComponent(L"Box", m_Box);
	g_GameObject.GetGameObject(L"Box")->InsertComponent(new myController);

	for (int iBox = 0; iBox < NUM_OBJECTS; iBox++)
	{
		//m_QuadTree.AddObject(&m_pObject[iBox]);
	}


	if (!m_Box->Create(L"vs.txt", L"ps.txt",
		L"../../data/bitmap/intro.bmp"))
	{
		return false;
	}
	m_Box->m_pTransform->m_matScale._11 = 2.0f;
	m_Box->m_pTransform->m_matScale._22 = 2.0f;
	m_Box->m_pTransform->m_matScale._33 = 2.0f;
	myModelViewCamera* pModelCamera = new myModelViewCamera;
	g_CamMgr.CreateCameraObj(L"ModelCamera", pModelCamera);
	//프러스텀 달기
	pModelCamera->CreateViewMatrix({ 0,5,-5 }, { 0,0,0 });
	float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	pModelCamera->CreateProjMatrix(1, 1000, PI2D, fAspect);
	g_CamMgr.SetMainCamera(L"ModelCamera");

	m_TopCamera = new myCamera;
	g_CamMgr.CreateCameraObj(L"TopCamera", m_TopCamera);

	m_TopCamera->CreateViewMatrix({ 0,30.0f,-0.1f }, { 0,0,0 });
	fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	m_TopCamera->CreateOrthographic(desc.iNumCols, desc.iNumRows, 1.0f, 1000);
	return true;
}

bool Sample::Frame()
{
	//Matrix matScale;
	//Matrix matRotation;

	//matScale = Matrix::CreateScale(1, 1, 1);
	//matRotation = Matrix::CreateRotationY(g_fGameTimer);
	//m_matBoxWorld._41 = m_Box->m_pTransform->m_vPos.x;
	//m_matBoxWorld._42 = m_Box->m_pTransform->m_vPos.y;
	//m_matBoxWorld._43 = m_Box->m_pTransform->m_vPos.z;
	//m_matBoxWorld = matScale * matRotation;
	//m_matBoxWorld._42 = 3.0f;

	if (g_Input.GetKey('0') == KEY_PUSH)
	{
		myDxState::m_FillMode = D3D11_FILL_WIREFRAME;
		myDxState::SetRasterizerState();
	}
	if (g_Input.GetKey('9') == KEY_PUSH)
	{
		myDxState::m_FillMode = D3D11_FILL_SOLID;
		myDxState::SetRasterizerState();
	}
	if (g_Input.GetKey('8') == KEY_PUSH)
	{
		myDxState::m_CullMode = D3D11_CULL_BACK;
		myDxState::SetRasterizerState();
	}
	if (g_Input.GetKey('7') == KEY_PUSH)
	{
		myDxState::m_CullMode = D3D11_CULL_FRONT;
		myDxState::SetRasterizerState();
	}
	if (g_Input.GetKey('6') == KEY_PUSH)
	{
		g_CamMgr.SetMainCamera(L"DebugCamera");
	}
	if (g_Input.GetKey('5') == KEY_PUSH)
	{
		g_CamMgr.SetMainCamera(L"ModelCamera");
	}
	if (g_Input.GetKey('4') == KEY_PUSH)
	{
		g_CamMgr.SetMainCamera(L"TopCamera");
	}
	m_Box->m_pTransform->m_vPos.y = m_Map->GetHeight(m_Box->m_pTransform->m_matWorld._41, m_Box->m_pTransform->m_matWorld._43);
	g_pMainCamTransform->SetTarget(m_Box->m_pTransform->m_vPos);
	g_CamMgr.m_pMainCamera->FrameFrustum();
	return true;
}

bool Sample::Render()
{
	//최종적으로는 모든 함수는 각자의 Render에서 돌아가게끔 해야함
	//그걸 호출하는건 obj매니저에 있는 메인gameobject

	//IA에 그려줄 타입 설정
	g_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//레스터라이저 스테이트 세팅
	g_pImmediateContext->RSSetState(myDxState::m_pRS.Get());
	//픽셀 섀이더에 샘플러 세팅(보간법)
	g_pImmediateContext->PSSetSamplers(0, 1, myDxState::m_pWrapLinear.GetAddressOf());
	//뎁스 스탠실 스테이트 세팅(깊이값 버퍼)
	g_pImmediateContext->OMSetDepthStencilState(myDxState::m_pDSS.Get(), 0);

	//// CULLING
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
	//	g_pImmediateContext->UpdateSubresource(
	//		m_Map->m_pIndexBuffer.Get(), 0, NULL, &visibleIB.at(0), 0, 0);
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

		m_Box->m_pTransform->SetMatrix(NULL,
			&m_TopCamera->m_pTransform->m_matView,
			&m_TopCamera->m_pTransform->m_matProj);
		m_Box->Render();

		DrawObject(&m_TopCamera->m_pTransform->m_matView,
			&m_TopCamera->m_pTransform->m_matProj);


		m_MiniMap->End();
	}

	DrawQuadLine(m_QuadTree.m_pRootNode);
	DrawObject(&g_pMainCamTransform->m_matView,
		&g_pMainCamTransform->m_matProj);

	////그리기
	m_Box->m_pTransform->SetMatrix(NULL,
		&g_pMainCamTransform->m_matView, 
		&g_pMainCamTransform->m_matProj);
	//m_Box->Render();

	m_Map->m_pTransform->SetMatrix(NULL,
		&g_pMainCamTransform->m_matView,
		&g_pMainCamTransform->m_matProj);
	//m_Map->Render();

	
	m_MiniMap->m_pTransform->SetMatrix(NULL,
		NULL, //&m_pMainCamera->m_matView,
		NULL); //&m_pMainCamera->m_matProj);
	//m_MiniMap->Render();



	return true;
}

bool Sample::Release()
{
	//m_MiniObj->Release();
	//SAFE_DELETE_ARRAY(m_pObject);
	//m_QuadTree.Release();
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

bool Sample::DrawQuadLine(myNode * pNode)
{
	if (pNode == NULL) return true;

	if (m_QuadTree.m_iRenderDepth >= pNode->m_dwDepth)
		//if (4 >= pNode->m_dwDepth)
	{
		m_pBasisLine->m_pTransform->SetMatrix(NULL,
			&g_pMainCamTransform->m_matView,
			&g_pMainCamTransform->m_matProj);

		Vector4 vColor = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
		if (pNode->m_dwDepth == 0) vColor = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
		if (pNode->m_dwDepth == 1) vColor = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
		if (pNode->m_dwDepth == 2) vColor = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
		if (pNode->m_dwDepth == 3) vColor = Vector4(1.0f, 0.0f, 1.0f, 1.0f);
		if (pNode->m_dwDepth == 4) vColor = Vector4(1.0f, 1.0f, 0.0f, 1.0f);
		if (pNode->m_dwDepth == 5) vColor = Vector4(0.0f, 0.5f, 1.0f, 1.0f);
		if (pNode->m_dwDepth == 6) vColor = Vector4(1.0f, 0.5f, 0.0f, 1.0f);
		if (pNode->m_dwDepth == 7) vColor = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
		if (pNode->m_dwDepth == 8) vColor = Vector4(1.0f, 0.5f, 0.5f, 1.0f);
		if (pNode->m_dwDepth == 9) vColor = Vector4(1.0f, 0.5f, 1.0f, 1.0f);

		Vector3 vPoint[4];
		vPoint[0] = Vector3(pNode->m_myBox.vMin.x, pNode->m_myBox.vMax.y, pNode->m_myBox.vMax.z);
		vPoint[0].y -= 1.0f * pNode->m_dwDepth;
		vPoint[1] = Vector3(pNode->m_myBox.vMax.x, pNode->m_myBox.vMax.y, pNode->m_myBox.vMax.z);
		vPoint[1].y -= 1.0f * pNode->m_dwDepth;
		vPoint[2] = Vector3(pNode->m_myBox.vMin.x, pNode->m_myBox.vMax.y, pNode->m_myBox.vMin.z);
		vPoint[2].y -= 1.0f * pNode->m_dwDepth;
		vPoint[3] = Vector3(pNode->m_myBox.vMax.x, pNode->m_myBox.vMax.y, pNode->m_myBox.vMin.z);
		vPoint[3].y -= 1.0f * pNode->m_dwDepth;

		m_pBasisLine->Draw(vPoint[0], vPoint[1], vColor);
		m_pBasisLine->Draw(vPoint[1], vPoint[3], vColor);
		m_pBasisLine->Draw(vPoint[2], vPoint[3], vColor);
		m_pBasisLine->Draw(vPoint[0], vPoint[2], vColor);
	}
	for (int iNode = 0; iNode < pNode->m_ChildList.size(); iNode++)
	{
		DrawQuadLine(pNode->m_ChildList[iNode]);
	}
	return true;
}

//void Sample::DrawObject(Matrix * pView, Matrix * pProj)
//{
//	for (int iBox = 0; iBox < NUM_OBJECTS; iBox++)
//	{
//		m_pObject[iBox].m_matWorld._42 =
//			m_Map->GetHeight(m_pObject[iBox].m_matWorld._41,
//				m_pObject[iBox].m_matWorld._43);
//
//		m_Box->m_pTransform->SetMatrix(&m_pObject[iBox].m_matWorld,
//			pView,
//			pProj);
//		// OBB와 프로스텀 박스의 제외처리( 걸쳐 있어도 TRUE가 됨. )
//		if (g_CamMgr.m_pMainCamera->m_Frustum.CheckOBBInPlane(&m_pObject[iBox].m_myBox))
//		{
//			m_Box->Render();
//		}
//	}
//}

