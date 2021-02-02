#include "myDevice.h"

HRESULT myDevice::CreateGIFactory()
{
	//����̽��� �����Ǿ� ���� ������ ����
	if (m_pd3dDevice == NULL) return E_FAIL;
	HRESULT hr;
	IDXGIDevice * pDXGIDevice;
	//GI����̽��� ������ uid��ȣ�� �ҷ��´�
	hr = m_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice);
	//GI���(�׷���ī��) ������ �ҷ��´�
	IDXGIAdapter * pDXGIAdapter;
	hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&pDXGIAdapter);
	//�� ���μ����� ���� GIFactory�� �ҷ��´�
	pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&m_pGIFactory);
	//�ҷ����µ� ����� ����̽��� ��͸� ������ ���ش�
	pDXGIDevice->Release();
	pDXGIAdapter->Release();
	return S_OK;
}

HRESULT myDevice::CreateDevice()
{
	//D2D RT
	//dx3d�� dx2d�� �����ϴ� �÷���
	UINT	Flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	//����� ��� �÷���
	Flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	//DirectX ����
	D3D_FEATURE_LEVEL	pFeatureLevels[] =
	{
		//D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0,
	};
	// �׷���ī��
	IDXGIAdapter*		 pAdapter = nullptr;
	//����� ����̹� Ÿ��
	D3D_DRIVER_TYPE		DriverType[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	HMODULE				Software = NULL; // �ܺθ�� �������

	//UINT				FeatureLevels = ARRAYSIZE(pFeatureLevels);
	UINT				FeatureLevels = sizeof(pFeatureLevels) / sizeof(pFeatureLevels[0]);
	UINT				SDKVersion = D3D11_SDK_VERSION;
	D3D_FEATURE_LEVEL   OutputFeatureLevel;

	HRESULT hr = S_OK;
	//����̽� ����
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
			&m_pd3dDevice,
			&OutputFeatureLevel,
			&m_pd3dContext);

		if (SUCCEEDED(hr))
		{
			break;
		}
	}
	return hr;
}

HRESULT myDevice::CreateSwapChain()
{
	//����ü�� ��������
	DXGI_SWAP_CHAIN_DESC pSwapChainDesc;
	ZeroMemory(&pSwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	//���� ������
	pSwapChainDesc.BufferDesc.Width = g_rtClient.right;
	pSwapChainDesc.BufferDesc.Height = g_rtClient.bottom;
	//���� Ÿ�� = RGBA 32��Ʈ�� ����ϰ� �븻������ ���� �ʴ´�
	pSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//������
	pSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	pSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	pSwapChainDesc.SampleDesc.Count = 1;
	//���� Ÿ�� ����
	pSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	//���� ����
	pSwapChainDesc.BufferCount = 1;
	//�ѷ��� ������ �ڵ�
	pSwapChainDesc.OutputWindow = g_hWnd;
	//â��� ����
	pSwapChainDesc.Windowed = true;
	//pSwapChainDesc.SwapEffect;
	//�÷��� �� ���
	pSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	//����ü�� ����
	HRESULT hr = m_pGIFactory->CreateSwapChain(
		m_pd3dDevice,
		&pSwapChainDesc,
		&m_pSwapChain);
	return hr;
}

HRESULT myDevice::SetRenderTargetView()
{
	//����۸� ����ش�
	ID3D11Texture2D* pBackBuffer = nullptr;
	//����ü�ο��� ����۸� �����´�
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		(LPVOID*)&pBackBuffer);
	//���� Ÿ���� ����۸� ����ؼ� ������ش�
	HRESULT hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL,
		&m_pRednerTargetView);
	//����۸� ����� �� ������ ���ش�
	if (pBackBuffer) pBackBuffer->Release();
	return hr;
}

bool myDevice::SetViewport()
{
	//����Ʈ ����
	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
	m_Viewport.Width = WINDOWSIZEX;
	m_Viewport.Height = WINDOWSIZEY;
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
	if (SetViewport() == false)
	{
		return false;
	}
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
	return true;
}

bool myDevice::PreRender()
{
	if (m_pd3dContext)
	{
		//������ ���������� �ƿ�ǲ ���տ� ����Ÿ���� �������ش�
		m_pd3dContext->OMSetRenderTargets(1, &m_pRednerTargetView, NULL);
		/*float clearColor[] = { cosf(g_fGameTimer)*0.5f + 0.5f,
								-cosf(g_fGameTimer)*0.5f + 0.5f,
								sinf(g_fGameTimer)*0.5f + 0.5f,1 };*/
		float clearColor[] = { 0,0,0,1 };
		//����Ÿ���� �ʱ�ȭ���ش�
		m_pd3dContext->ClearRenderTargetView(m_pRednerTargetView, clearColor);
		//ȭ�� ��ǥ�� ��ȯ���ش�
		m_pd3dContext->RSSetViewports(1, &m_Viewport);
	}
	return true;
}

bool myDevice::Render()
{
	return false;
}

bool myDevice::PostRender()
{
	//����ۿ� ����Ʈ ���۸� �������ش�
	if (m_pSwapChain)
	{
		m_pSwapChain->Present(0, 0);
	}
	return true;
}

bool myDevice::Release()
{
	m_pRednerTargetView->Release();
	m_pSwapChain->Release();
	m_pd3dContext->Release();
	m_pd3dDevice->Release();
	m_pGIFactory->Release();
	return true;
}

void myDevice::SetMode(bool bFullScreen)
{
	//���� ������ ��Ŀ���� ���� �ʰ� �ؾ���
	m_bFullScreen = bFullScreen;
	//Ǯ��ũ�� ��ȯ
	m_pSwapChain->SetFullscreenState(m_bFullScreen, NULL);
	//Ǯ��ũ���� �ƴϸ� ������ ����
	if (m_bFullScreen == FALSE)
	{
		ShowWindow(m_hWnd, SW_SHOW);
	}
}

void myDevice::ResizeDevice(UINT w, UINT h)
{
	//����̽��� �����Ǿ����� ������ ����
	if (m_pd3dDevice == NULL)  return;
	//���� ���ҽ��� ����
	DeleteDXResource();
	//����Ÿ�� �ʱ�ȭ
	m_pd3dContext->OMSetRenderTargets(0, NULL, NULL);
	//����Ÿ�� ������
	if (m_pRednerTargetView) m_pRednerTargetView->Release();
	DXGI_SWAP_CHAIN_DESC pSwapChainDesc;
	//����ü�� ���� ��������
	m_pSwapChain->GetDesc(&pSwapChainDesc);
	//���� ������ ����
	m_pSwapChain->ResizeBuffers(
		pSwapChainDesc.BufferCount,
		w,
		h,
		pSwapChainDesc.BufferDesc.Format,
		pSwapChainDesc.Flags);
	//����Ÿ�� ����
	SetRenderTargetView();
	//����Ʈ ����
	SetViewport();
	//���ҽ� ����
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
