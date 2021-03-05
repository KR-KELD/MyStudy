#include "Sample.h"
#define NUM_OBJECTS 10
GAMERUN;

bool Sample::Init()
{
	HRESULT hr = NULL;


	m_Map = new myHeightMap;
	g_ObjMgr.CreateObjComponent(L"Map", m_Map);
	m_Map->CreateHeightMap(L"../../data/heightMap513.bmp");


	myMapDesc desc;
	desc.iNumCols = m_Map->m_iNumCols;
	desc.iNumRows = m_Map->m_iNumRows;
	desc.fCellDistance = 1;
	desc.fScaleHeight = 10.0f;
	desc.szTexFile = L"../../data/castle.jpg";
	desc.szVS = L"../../data/shader/VS.txt";
	desc.szPS = L"../../data/shader/PS.txt";
	m_Map->CreateMap(desc);
	m_Map->m_isRender = false;
	//m_Map->CalNormal();
	m_QuadTree.CreateQuadTree(m_Map);

	return true;
}

bool Sample::Frame()
{
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

	m_Map->m_pTransform->SetMatrix(NULL,
		&g_pMainCamTransform->m_matView,
		&g_pMainCamTransform->m_matProj);
	//m_Map->Render();
	m_QuadTree.Render();


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

//bool Sample::DrawQuadLine(myNode * pNode)
//{
//	if (pNode == NULL) return true;
//
//	if (m_QuadTree.m_iRenderDepth >= pNode->m_dwDepth)
//		//if (4 >= pNode->m_dwDepth)
//	{
//		m_pBasisLine->m_pTransform->SetMatrix(NULL,
//			&g_pMainCamTransform->m_matView,
//			&g_pMainCamTransform->m_matProj);
//
//		Vector4 vColor = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
//		if (pNode->m_dwDepth == 0) vColor = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
//		if (pNode->m_dwDepth == 1) vColor = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
//		if (pNode->m_dwDepth == 2) vColor = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
//		if (pNode->m_dwDepth == 3) vColor = Vector4(1.0f, 0.0f, 1.0f, 1.0f);
//		if (pNode->m_dwDepth == 4) vColor = Vector4(1.0f, 1.0f, 0.0f, 1.0f);
//		if (pNode->m_dwDepth == 5) vColor = Vector4(0.0f, 0.5f, 1.0f, 1.0f);
//		if (pNode->m_dwDepth == 6) vColor = Vector4(1.0f, 0.5f, 0.0f, 1.0f);
//		if (pNode->m_dwDepth == 7) vColor = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
//		if (pNode->m_dwDepth == 8) vColor = Vector4(1.0f, 0.5f, 0.5f, 1.0f);
//		if (pNode->m_dwDepth == 9) vColor = Vector4(1.0f, 0.5f, 1.0f, 1.0f);
//
//		Vector3 vPoint[4];
//		vPoint[0] = Vector3(pNode->m_myBox.vMin.x, pNode->m_myBox.vMax.y, pNode->m_myBox.vMax.z);
//		vPoint[0].y -= 1.0f * pNode->m_dwDepth;
//		vPoint[1] = Vector3(pNode->m_myBox.vMax.x, pNode->m_myBox.vMax.y, pNode->m_myBox.vMax.z);
//		vPoint[1].y -= 1.0f * pNode->m_dwDepth;
//		vPoint[2] = Vector3(pNode->m_myBox.vMin.x, pNode->m_myBox.vMax.y, pNode->m_myBox.vMin.z);
//		vPoint[2].y -= 1.0f * pNode->m_dwDepth;
//		vPoint[3] = Vector3(pNode->m_myBox.vMax.x, pNode->m_myBox.vMax.y, pNode->m_myBox.vMin.z);
//		vPoint[3].y -= 1.0f * pNode->m_dwDepth;
//
//		m_pBasisLine->Draw(vPoint[0], vPoint[1], vColor);
//		m_pBasisLine->Draw(vPoint[1], vPoint[3], vColor);
//		m_pBasisLine->Draw(vPoint[2], vPoint[3], vColor);
//		m_pBasisLine->Draw(vPoint[0], vPoint[2], vColor);
//	}
//	for (int iNode = 0; iNode < pNode->m_ChildList.size(); iNode++)
//	{
//		DrawQuadLine(pNode->m_ChildList[iNode]);
//	}
//	return true;
//}

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

