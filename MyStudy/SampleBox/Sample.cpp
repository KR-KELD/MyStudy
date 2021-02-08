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
	hr = m_pd3dDevice->CreateTexture2D(&texDesc, NULL, &pTexture);
	if (FAILED(hr))
	{
		return false;
	}
	//뎁스 스텐실 - 1개
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	hr = m_pd3dDevice->CreateDepthStencilView(pTexture, &dsvDesc, &m_pDSV);
	//다 쓴 텍스쳐 릴리즈
	if (pTexture)pTexture->Release();
	if (FAILED(hr))
	{
		return false;
	}
	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = TRUE;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	hr = m_pd3dDevice->CreateDepthStencilState(&dssDesc, &m_pDSS);
	if (FAILED(hr))
	{
		return false;
	}
	// load texture
	//ID3D11Resource* texture;
	//텍스쳐 불러오기
	//hr = DirectX::CreateWICTextureFromFile(
	//	m_pd3dDevice, L"../../data/bitmap/flametank.bmp",
	//	NULL,
	//	&m_pTextureSRV);

	//텍스쳐 - 공유가능
	//hr = DirectX::CreateWICTextureFromFile(
	//	m_pd3dDevice, L"../../data/main_start_nor.png",
	//	NULL,
	//	&m_pTextureSRV);

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
	if (FAILED(hr))
	{
		return false;
	}


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
	//myVector3 temp = { -3.0f, 0.0f, 0.0f };
	//m_Box[1].SetBox(temp);
	//temp = { 3.0f,0.0f,0.0f };
	//m_Box[2].SetBox(temp);
	//for (int i = 0; i < 3; i++)
	//{
	//	m_Box[i].Init(m_pd3dDevice);
	//}

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
	m_pd3dContext->RSSetState(m_pRS);
	//픽셀 섀이더에 샘플러 세팅(보간법)
	m_pd3dContext->PSSetSamplers(0, 1, &m_pWrapLinear);
	//뎁스 스탠실 스테이트 세팅(깊이값 버퍼)
	m_pd3dContext->OMSetDepthStencilState(m_pDSS, 0);
	//그리기
	m_Box.SetMatrix(&m_matBoxWorld, &m_matView, &m_matProj);
	m_Box.Render(m_pd3dContext);

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
	m_pDSV->Release();
	m_pDSS->Release();
	m_pWrapLinear->Release();
	m_pRS->Release();
	m_pRSSolidBack->Release();
	m_pRSWireBack->Release();
	m_Box.Relase();
	m_Plane.Relase();
	m_Line.Relase();
	return true;
}

bool Sample::PreRender()
{
	myCore::PreRender();
	ID3D11RenderTargetView* pNullRTV = NULL;
	if (m_pd3dContext)
	{
		//랜더링 파이프라인 아웃풋 병합에 랜더타겟을 설정해준다
		m_pd3dContext->OMSetRenderTargets(1, &pNullRTV, m_pDSV);
		m_pd3dContext->OMSetRenderTargets(
			1, &m_pRednerTargetView,
			m_pDSV);
		float clearColor[] = { 0,0,0,1 };
		//랜더타겟을 초기화해준다
		m_pd3dContext->ClearDepthStencilView(
			m_pDSV, D3D10_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
		////화면 좌표로 변환해준다
		//m_pd3dContext->RSSetViewports(1, &m_Viewport);
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
