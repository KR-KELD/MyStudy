#include "Sample.h"
GAMERUN;
bool Sample::Init()
{
	HRESULT hr = NULL;

	//카메라
	m_matWorld.Identity();
	myVector3 p = m_vCameraPos;
	myVector3 t = m_vCameraTarget;
	myVector3 u = { 0,1,0 };
	m_matView.CreateViewLook(p, t, u);
	float fN = 1;
	float fF = 1000;
	float fFov = PI / 2.0f;
	float fAspect = g_rtClient.right / g_rtClient.bottom;
	m_matProj.PerspectiveFovLH(fN, fF, fFov, fAspect);

	//뎁스 텍스쳐 - 1개
	//뎁스 텍스쳐를 만들어라
	ID3D11Texture2D* pTexture = nullptr;
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
	texDesc.Width = g_rtClient.right;
	texDesc.Height = g_rtClient.bottom;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	m_pd3dDevice->CreateTexture2D(&texDesc, NULL, &pTexture);

	//뎁스 스텐실 - 1개
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	m_pd3dDevice->CreateDepthStencilView(pTexture, &dsvDesc, &m_pDSV);

	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = TRUE;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;


	m_pd3dDevice->CreateDepthStencilState(&dssDesc, &m_pDSS);
	// load texture
	//ID3D11Resource* texture;
	//텍스쳐 불러오기
	//hr = DirectX::CreateWICTextureFromFile(
	//	m_pd3dDevice, L"../../data/bitmap/flametank.bmp",
	//	NULL,
	//	&m_pTextureSRV);

	//텍스쳐 - 공유가능
	hr = DirectX::CreateWICTextureFromFile(
		m_pd3dDevice, L"../../data/main_start_nor.png",
		NULL,
		&m_pTextureSRV);
	//샘플러 옵션 세팅(보간법 세팅)
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	//축소 확대 밉맵에서 선형보간 사용
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//
	samplerDesc.BorderColor[0] = 1;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 1;
	//Lod에 사용할 밉맵 최소 최대값
	samplerDesc.MinLOD = FLT_MIN;
	samplerDesc.MaxLOD = FLT_MAX;
	//샘플러 세팅
	hr = m_pd3dDevice->CreateSamplerState(&samplerDesc, &m_pWrapLinear);


	// Rasterizer State
	//레스터라이저 - 공유가능
	//그리기 모드
	m_FillMode = D3D11_FILL_SOLID;
	//컬링 모드
	m_CullMode = D3D11_CULL_BACK;
	//레스터라이저 스테이트 세팅
	SetRasterizerState();

	//레스터라이저 옵션 세팅
	D3D11_RASTERIZER_DESC rdesc;
	ZeroMemory(&rdesc, sizeof(D3D11_RASTERIZER_DESC));
	rdesc.FillMode = D3D11_FILL_SOLID;
	rdesc.CullMode = D3D11_CULL_BACK;
	hr = m_pd3dDevice->CreateRasterizerState(&rdesc, &m_pRSSolidBack);
	if (FAILED(hr))
	{
		return false;
	}

	ZeroMemory(&rdesc, sizeof(D3D11_RASTERIZER_DESC));
	rdesc.FillMode = D3D11_FILL_WIREFRAME;
	rdesc.CullMode = D3D11_CULL_BACK;
	hr = m_pd3dDevice->CreateRasterizerState(&rdesc, &m_pRSWireBack);
	if (FAILED(hr))
	{
		return false;
	}

	//버텍스 버퍼 - 공유불가능
	//MakeBox();
	//버텍스 리스트 만들기
	m_VertexList.resize(4);
	m_VertexList[0] = {
		myVector3(-1.0f, 1.0f, 0.5f),
		myVector3(0,0,0),
		myVector4(1,0,0,1),
		myVector2(0,0) };
	m_VertexList[1] = {
		myVector3(1.0f, 1.0f, 0.5f),
		myVector3(0,0,0),
		myVector4(0,1,0,1),
		myVector2(3,0) };
	m_VertexList[2] = {
		myVector3(-1.0f, -1.0f, 0.5f),
		myVector3(0,0,0),
		myVector4(0,0,1,1),
		myVector2(0,3) };
	m_VertexList[3] = {
		myVector3(1.0f, -1.0f, 0.5f),
		myVector3(0,0,0),
		myVector4(1,1,1,1),
		myVector2(3,3) };


	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	//GPU에 할당할 버텍스의 크기X갯수
	bd.ByteWidth = sizeof(myDataCB);
	//접근 방법
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//버텍스 버퍼 플래그 설정
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;


	//
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	//시스템 메모리의 시작주소 넘겨주기
	sd.pSysMem = &m_cbData;
	//버텍스 버퍼 생성
	hr = m_pd3dDevice->CreateBuffer(&bd, &sd, &m_pConstantBuffer);
	if (FAILED(hr))
	{
		return false;
	}
	//버퍼 옵션 설정
	//D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	//GPU에 할당할 버텍스의 크기X갯수
	bd.ByteWidth = sizeof(MY_VERTEX) * m_VertexList.size();
	//
	bd.Usage = D3D11_USAGE_DEFAULT;
	//버텍스 버퍼 플래그 설정
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;


	//
	//D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	//시스템 메모리의 시작주소 넘겨주기
	sd.pSysMem = &m_VertexList.at(0);
	//버텍스 버퍼 생성
	hr = m_pd3dDevice->CreateBuffer(&bd, &sd, &m_pVertexBuffer);
	if (FAILED(hr))
	{
		return false;
	}
	//인덱스 버퍼 - 공유불가능
	//인덱스 리스트 만들기
	m_IndexList.resize(6);
	m_IndexList[0] = 0;
	m_IndexList[1] = 1;
	m_IndexList[2] = 2;
	m_IndexList[3] = 2;
	m_IndexList[4] = 1;
	m_IndexList[5] = 3;

	//버퍼 옵션 초기화
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	//인덱스 버퍼 사이즈 넘기기
	bd.ByteWidth = sizeof(DWORD) * m_IndexList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	//인덱스 버퍼 플래그 설정
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_IndexList.at(0);
	//인덱스 버퍼 생성
	hr = m_pd3dDevice->CreateBuffer(&bd, &sd, &m_pIndexBuffer);
	if (FAILED(hr))
	{
		return false;
	}
	//픽셀,버텍스 섀이더 - 공유가능
	//섀이더,에러 블롭 생성
	ID3DBlob* pVSObj;
	ID3DBlob* pPSObj;
	ID3DBlob* pErrorMsgs;
	//파일로부터 버텍스 섀이더 함수를 가져온다
	hr = D3DCompileFromFile(L"vs.txt", NULL, NULL, "VS", "vs_5_0", 0, 0, &pVSObj, &pErrorMsgs);
	if (FAILED(hr))
	{
		CompilerCheck(pErrorMsgs);
		return false;
	}
	//가져온 함수로 버텍스 섀이더를 생성
	hr = m_pd3dDevice->CreateVertexShader(pVSObj->GetBufferPointer(), pVSObj->GetBufferSize(), NULL, &m_pVertexShader);
	//파일로부터 픽셀 섀이더 함수를 가져온다
	hr = D3DCompileFromFile(L"ps.txt", NULL, NULL, "PS", "ps_5_0", 0, 0, &pPSObj, &pErrorMsgs);
	if (FAILED(hr))
	{
		CompilerCheck(pErrorMsgs);
		return false;
	}
	//가져온 함수로 픽셀 섀이더 생성
	hr = m_pd3dDevice->CreatePixelShader(pPSObj->GetBufferPointer(), pPSObj->GetBufferSize(), NULL, &m_pPixelShader);

	//섀이더 함수에 전달할 변수의 옵션 설정
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE",  0, DXGI_FORMAT_R32G32_FLOAT, 0, 40,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT iNumElement = sizeof(layout) / sizeof(layout[0]);
	//인풋 레이아웃 생성
	hr = m_pd3dDevice->CreateInputLayout(
		layout,
		iNumElement,
		pVSObj->GetBufferPointer(),
		pVSObj->GetBufferSize(),
		&m_pInputLayout
	);
	m_Box.Init(m_pd3dDevice);
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

	D3D11_MAPPED_SUBRESOURCE mr;

	HRESULT hr = m_pd3dContext->
		Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr);
	if (SUCCEEDED(hr))
	{
		myDataCB* pData = (myDataCB*)mr.pData;
		pData->matWorld = m_matWorld.Transpose();
		pData->matView = m_matView.Transpose();
		pData->matProject = m_matProj.Transpose();

		pData->vColor[0] = cosf(g_fGameTimer);
		pData->vColor[1] = sinf(g_fGameTimer);
		pData->vColor[2] = 1.0f - cosf(g_fGameTimer);
		pData->vColor[3] = 1;
		pData->vTime[0] = cosf(g_fGameTimer) * 0.5f + 0.5f;
		pData->vTime[1] = g_fGameTimer;
		m_pd3dContext->Unmap(m_pConstantBuffer, 0);
	}
	return true;
}

bool Sample::Render()
{
	//자를 단위
	UINT iStride = sizeof(MY_VERTEX);
	//시작 위치
	UINT iOffset = 0;
	//IA 버텍스 버퍼 설정
	m_pd3dContext->IASetVertexBuffers(0, 1,
		&m_pVertexBuffer, &iStride, &iOffset);
	//IA 인덱스 버퍼 설정
	m_pd3dContext->IASetIndexBuffer(m_pIndexBuffer,
		DXGI_FORMAT_R32_UINT, 0);
	//IA 세팅
	m_pd3dContext->IASetInputLayout(m_pInputLayout);
	//콘스턴트 버퍼 세팅
	m_pd3dContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pd3dContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	//버텍스 섀이더 세팅
	m_pd3dContext->VSSetShader(m_pVertexShader, NULL, 0);
	//픽셀 섀이더 세팅
	m_pd3dContext->PSSetShader(m_pPixelShader, NULL, 0);
	//IA에 그려줄 타입 설정
	m_pd3dContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//m_pd3dContext->Draw(m_VertexList.size(), 0);
	//레스터라이저 스테이트 세팅
	m_pd3dContext->RSSetState(m_pRS);
	//픽셀 섀이더에 텍스쳐 전달
	m_pd3dContext->PSSetShaderResources(0, 1, &m_pTextureSRV);
	//픽셀 섀이더에 샘플러 세팅(보간법)
	m_pd3dContext->PSSetSamplers(0, 1, &m_pWrapLinear);
	//뎁스 스탠실 스테이트 세팅(깊이값 버퍼)
	m_pd3dContext->OMSetDepthStencilState(m_pDSS, 0);
	//그리기
	m_pd3dContext->DrawIndexed(m_IndexList.size(), 0, 0);
	m_Box.Render(m_pd3dContext);
	return true;
}

bool Sample::Release()
{
	m_Box.Release();
	m_pDSV->Release();
	m_pDSS->Release();
	m_pWrapLinear->Release();
	m_pTextureSRV->Release();
	m_pRS->Release();
	m_pRSSolidBack->Release();
	m_pRSWireBack->Release();
	m_pConstantBuffer->Release();
	m_pVertexBuffer->Release();
	m_pIndexBuffer->Release();
	m_pInputLayout->Release();
	m_pVertexShader->Release();
	m_pPixelShader->Release();
	return true;
}

bool Sample::PreRender()
{
	if (m_pd3dContext)
	{
		//랜더링 파이프라인 아웃풋 병합에 랜더타겟을 설정해준다
		m_pd3dContext->OMSetRenderTargets(1, &m_pRednerTargetView, m_pDSV);
		/*float clearColor[] = { cosf(g_fGameTimer)*0.5f + 0.5f,
								-cosf(g_fGameTimer)*0.5f + 0.5f,
								sinf(g_fGameTimer)*0.5f + 0.5f,1 };*/
		float clearColor[] = { 0,0,0,1 };
		//랜더타겟을 초기화해준다
		m_pd3dContext->ClearRenderTargetView(m_pRednerTargetView, clearColor);
		m_pd3dContext->ClearDepthStencilView(
			m_pDSV, D3D10_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
		//화면 좌표로 변환해준다
		m_pd3dContext->RSSetViewports(1, &m_Viewport);
	}
	return true;
}

bool Sample::MakeBox()
{
	m_VertexList.resize(24);
	myMatrix Rot;
	Rot._11 = cosf(PI2D);
	Rot._13 = sinf(PI2D);
	Rot._31 = -sinf(PI2D);
	Rot._33 = cosf(PI2D);
	m_VertexList[0] = {
		myVector3(-1.0f, 1.0f, -1.0f),
		myVector3(0,0,0),
		myVector4(1,0,0,1),
		myVector2(0,0) };
	m_VertexList[1] = {
		myVector3(1.0f, 1.0f, -1.0f),
		myVector3(0,0,0),
		myVector4(0,1,0,1),
		myVector2(1,0) };
	m_VertexList[2] = {
		myVector3(-1.0f, -1.0f, 1.0f),
		myVector3(0,0,0),
		myVector4(0,0,1,1),
		myVector2(0,1) };
	m_VertexList[3] = {
		myVector3(1.0f, -1.0f, 1.0f),
		myVector3(0,0,0),
		myVector4(1,1,1,1),
		myVector2(1,1) };

	for (int i = 1; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m_VertexList[j + i * 4].p = Rot * m_VertexList[(i - 1) * 4 + j].p;
		}
	}
	Rot.Identity();
	Rot._22 = cosf(PI2D);
	Rot._23 = -sinf(PI2D);
	Rot._32 = sinf(PI2D);
	Rot._33 = cosf(PI2D);
	for (int j = 0; j < 4; j++)
	{
		m_VertexList[j + 16].p = Rot * m_VertexList[j].p;
	}
	Rot.Identity();
	Rot._22 = cosf(PI2D);
	Rot._23 = sinf(PI2D);
	Rot._32 = -sinf(PI2D);
	Rot._33 = cosf(PI2D);
	for (int j = 0; j < 4; j++)
	{
		m_VertexList[j + 20].p = Rot * m_VertexList[j].p;
	}
	m_IndexList.resize(36);
	for (int i = 0; i < 6; i++)
	{
		m_IndexList[0 + i * 6] = 0 + i * 6;
		m_IndexList[1 + i * 6] = 1 + i * 6;
		m_IndexList[2 + i * 6] = 2 + i * 6;
		m_IndexList[3 + i * 6] = 2 + i * 6;
		m_IndexList[4 + i * 6] = 1 + i * 6;
		m_IndexList[5 + i * 6] = 3 + i * 6;
	}
	return true;
}

void Sample::CompilerCheck(ID3DBlob * pErrorMsgs)
{
	//블롭에서 메시지 가져오기
	C_STR szMsg = (char*)pErrorMsgs->GetBufferPointer();
	T_STR szError = to_mw(szMsg);
	MessageBox(NULL, szError.c_str(), L"ERROR", MB_OK);
}

void Sample::SetRasterizerState()
{
	HRESULT hr;
	// Rasterizer State
	if (m_pRS != nullptr) m_pRS->Release();
	D3D11_RASTERIZER_DESC rdesc;
	ZeroMemory(&rdesc, sizeof(D3D11_RASTERIZER_DESC));
	rdesc.FillMode = m_FillMode;
	rdesc.CullMode = m_CullMode;
	hr = m_pd3dDevice->CreateRasterizerState(&rdesc, &m_pRS);
	if (FAILED(hr))
	{
		return;
	}
}
