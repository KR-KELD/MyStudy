#include "myDevice.h"
ID3D11Device*	g_pd3dDevice = nullptr;
ID3D11DeviceContext*	g_pImmediateContext = nullptr;
HRESULT myDevice::CreateGIFactory()
{
	//디바이스가 생성되어 있지 않으면 리턴
	if (m_pd3dDevice.Get() == NULL) return E_FAIL;
	HRESULT hr;
	ComPtr<IDXGIDevice> pDXGIDevice;
	//GI디바이스의 정보를 uid번호로 불러온다
	hr = m_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)pDXGIDevice.GetAddressOf());
	//GI어뎁터(그래픽카드) 정보를 불러온다
	ComPtr<IDXGIAdapter> pDXGIAdapter;
	hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)pDXGIAdapter.GetAddressOf());
	//이 프로세스의 상위 GIFactory를 불러온다
	pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)m_pGIFactory.GetAddressOf());
	//불러오는데 사용한 디바이스와 어뎁터를 릴리즈 해준다
	return S_OK;
}

HRESULT myDevice::CreateDevice()
{
	//D2D RT
	//dx3d와 dx2d를 병행하는 플래그
	UINT	Flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	//디버그 모드 플래그
	Flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	//DirectX 버전
	D3D_FEATURE_LEVEL	pFeatureLevels[] =
	{
		//D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0,
	};
	// 그래픽카드
	IDXGIAdapter*		 pAdapter = nullptr;
	//사용할 드라이버 타입
	D3D_DRIVER_TYPE		DriverType[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	HMODULE				Software = NULL; // 외부모듈 사용유무

	//UINT				FeatureLevels = ARRAYSIZE(pFeatureLevels);
	UINT				FeatureLevels = sizeof(pFeatureLevels) / sizeof(pFeatureLevels[0]);
	UINT				SDKVersion = D3D11_SDK_VERSION;
	D3D_FEATURE_LEVEL   OutputFeatureLevel;

	HRESULT hr = S_OK;
	//디바이스 생성
	for (int iType = 0; iType < ARRAYSIZE(DriverType); iType++)
	{
		hr = D3D11CreateDevice(
			pAdapter,
			DriverType[iType],
			Software,
			Flags,
			pFeatureLevels,
			FeatureLevels,
			SDKVersion,
			m_pd3dDevice.GetAddressOf(),
			&OutputFeatureLevel,
			m_pd3dContext.GetAddressOf());

		if (SUCCEEDED(hr))
		{
			break;
		}
	}
	g_pd3dDevice = m_pd3dDevice.Get();
	g_pImmediateContext = m_pd3dContext.Get();
	return hr;
}

HRESULT myDevice::CreateSwapChain()
{
	//스왑체인 세부정보
	DXGI_SWAP_CHAIN_DESC pSwapChainDesc;
	ZeroMemory(&pSwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	//버퍼 사이즈
	pSwapChainDesc.BufferDesc.Width = g_rtClient.right;
	pSwapChainDesc.BufferDesc.Height = g_rtClient.bottom;
	//버퍼 타입 = RGBA 32비트를 사용하고 노말라이즈 하지 않는다
	pSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//프레임
	pSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	pSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	pSwapChainDesc.SampleDesc.Count = 1;
	//랜더 타겟 설정
	pSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	//버퍼 갯수
	pSwapChainDesc.BufferCount = 1;
	//뿌려줄 윈도우 핸들
	pSwapChainDesc.OutputWindow = g_hWnd;
	//창모드 여부
	pSwapChainDesc.Windowed = true;
	//pSwapChainDesc.SwapEffect;
	//플래그 값 사용
	pSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	//스왑체인 생성
	HRESULT hr = m_pGIFactory->CreateSwapChain(
		m_pd3dDevice.Get(),
		&pSwapChainDesc,
		m_pSwapChain.GetAddressOf());
	return hr;
}

HRESULT myDevice::SetRenderTargetView()
{
	//백버퍼를 비워준다
	ComPtr<ID3D11Texture2D> pBackBuffer = nullptr;
	//스왑체인에서 백버퍼를 가져온다
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		(LPVOID*)pBackBuffer.GetAddressOf());
	//랜더 타겟을 백버퍼를 사용해서 만들어준다
	HRESULT hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer.Get(), NULL,
		m_pRednerTargetView.GetAddressOf());
	//백버퍼를 사용한 뒤 릴리즈 해준다
	return hr;
}

HRESULT myDevice::SetDepthStencilView()
{
	// create depth texture
	ComPtr<ID3D11Texture2D> pTexture = nullptr;
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
	HRESULT hr = m_pd3dDevice->CreateTexture2D(&texDesc, NULL, pTexture.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	hr = m_pd3dDevice->CreateDepthStencilView(
		pTexture.Get(),
		&dsvDesc,
		m_pDSV.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool myDevice::SetViewport()
{
	//뷰포트 설정
	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
	m_Viewport.Width = g_rtClient.right;
	m_Viewport.Height = g_rtClient.bottom;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;
	return true;
}

bool myDevice::Init()
{
	if (FAILED(CreateDevice()))
	{
		return false;
	}
	if (FAILED(CreateGIFactory()))
	{
		return false;
	}
	if (FAILED(CreateSwapChain()))
	{
		return false;
	}
	if (FAILED(SetRenderTargetView()))
	{
		return false;
	}
	if (FAILED(SetDepthStencilView()))
	{
		return false;
	}
	if (SetViewport() == false)
	{
		return false;
	}
	myDxState::SetState(g_pd3dDevice);
	if (FAILED(m_pGIFactory->MakeWindowAssociation(m_hWnd,
		DXGI_MWA_NO_WINDOW_CHANGES |
		DXGI_MWA_NO_ALT_ENTER)))
	{
		return false;
	}
	return true;
}

bool myDevice::Frame()
{
	//if (m_pd3dContext.Get())
	//{
	//	m_pd3dContext->RSSetViewports(1, &m_Viewport);
	//	m_pd3dContext->OMSetRenderTargets(1, m_pRednerTargetView.GetAddressOf(), m_pDSV.Get());
	//	float clearColor[] = { 0,0,0,1 };
	//	m_pd3dContext->ClearRenderTargetView(m_pRednerTargetView.Get(), clearColor);
	//	m_pd3dContext->ClearDepthStencilView(m_pDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	//}
	return true;
}

bool myDevice::PreRender()
{
	if (m_pd3dContext.Get())
	{
		m_pd3dContext->RSSetViewports(1, &m_Viewport);
		//랜더링 파이프라인 아웃풋 병합에 랜더타겟을 설정해준다
		m_pd3dContext->OMSetRenderTargets(1, m_pRednerTargetView.GetAddressOf(), m_pDSV.Get());
		float clearColor[] = { 0.1f,0.1f,0.1f,1 };
		//랜더타겟을 초기화해준다
		m_pd3dContext->ClearRenderTargetView(m_pRednerTargetView.Get(), clearColor);
		m_pd3dContext->ClearDepthStencilView(m_pDSV.Get(), D3D10_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
		//화면 좌표로 변환해준다

		//IA에 그려줄 타입 설정
		m_pd3dContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
	return true;
}

bool myDevice::Render()
{
	return false;
}

bool myDevice::PostRender()
{
	//백버퍼와 프론트 버퍼를 스왑해준다
	if (m_pSwapChain.Get())
	{
		m_pSwapChain->Present(0, 0);
	}
	return true;
}

bool myDevice::Release()
{
	myDxState::Release();

	//m_pDSV->Release();
	//m_pRednerTargetView->Release();
	//m_pSwapChain->Release();
	//m_pd3dContext->Release();
	//m_pd3dDevice.Get()->Release();
	//m_pGIFactory->Release();
	return true;
}

void myDevice::SetMode(bool bFullScreen)
{
	//주의 윈도우 포커스를 놓지 않게 해야함
	m_bFullScreen = bFullScreen;
	//풀스크린 전환
	m_pSwapChain->SetFullscreenState(m_bFullScreen, NULL);
	//풀스크린이 아니면 윈도우 띄우기
	if (m_bFullScreen == FALSE)
	{
		ShowWindow(m_hWnd, SW_SHOW);
	}
}

void myDevice::ResizeDevice(UINT w, UINT h)
{
	//디바이스가 설정되어있지 않으면 리턴
	if (m_pd3dDevice.Get() == NULL)  return;
	//기존 리소스를 제거
	DeleteDXResource();
	HRESULT hr = S_OK;
	//랜더타겟 초기화
	m_pd3dContext->OMSetRenderTargets(0, NULL, NULL);
	//랜더타겟 릴리즈
	if (m_pRednerTargetView.Get()) m_pRednerTargetView->Release();
	if (m_pDSV.Get()) m_pDSV->Release();

	//스왑체인 설정 가져오기
	m_pSwapChain->GetDesc(&m_pSwapChainDesc);
	//버퍼 사이즈 조절
	if (FAILED(m_pSwapChain->ResizeBuffers(
		m_pSwapChainDesc.BufferCount,
		w,
		h,
		m_pSwapChainDesc.BufferDesc.Format,
		m_pSwapChainDesc.Flags)))
	{

	}
	m_pSwapChain->GetDesc(&m_pSwapChainDesc);
	m_rtClient.right = m_pSwapChainDesc.BufferDesc.Width;
	m_rtClient.bottom = m_pSwapChainDesc.BufferDesc.Height;
	g_rtClient = m_rtClient;
	//랜더타겟 세팅
	SetRenderTargetView();
	SetDepthStencilView();
	//뷰포트 세팅
	SetViewport();
	//리소스 생성
	CreateDXResource(w, h);
}

HRESULT myDevice::DeleteDXResource()
{
	return S_OK;
}

HRESULT myDevice::CreateDXResource(UINT w, UINT h)
{
	return S_OK;
}

myDevice::myDevice()
{
	m_pGIFactory = nullptr;
	m_pd3dDevice = nullptr;
	m_pd3dContext = nullptr;
	m_pSwapChain = nullptr;
	m_pRednerTargetView = nullptr;
}

myDevice::~myDevice()
{
}
