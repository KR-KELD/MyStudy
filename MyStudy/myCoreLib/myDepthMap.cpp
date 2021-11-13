#include "myDepthMap.h"
DECLARE_COMPONENT(myDepthMap)

bool myDepthMap::Init()
{
	CreateDepthMapRT(1024, 1024);
	m_matShadowTex._11 = 0.5f; m_matShadowTex._22 = -0.5f;
	m_matShadowTex._41 = 0.5f; m_matShadowTex._42 = 0.5f;
	CreateDepthVS(L"../../data/shader/DepthMap.hlsl");
	CreateDepthPS(L"../../data/shader/DepthMap.hlsl");
	return true;
}

bool myDepthMap::Frame()
{
	return true;
}

bool myDepthMap::PreFrame()
{
	return true;
}

bool myDepthMap::Render(ID3D11DeviceContext * pd3dContext)
{
	return true;
}

bool myDepthMap::Release()
{
	SAFE_DEL(m_pRT);
	return true;
}

bool myDepthMap::CreateDepthMapRT(int iTexWidth, int iTexHeight)
{
	m_pRT = new myDxRT;

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = iTexWidth;
	desc.Height = iTexHeight;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET |
		D3D11_BIND_SHADER_RESOURCE;

	m_pRT->SetRenderTargetView(&desc);

	desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	m_pRT->SetDepthStencilView(&desc);
	m_pRT->SetViewport();
	return true;
}

bool myDepthMap::CreateDepthVS(const TCHAR * szFileName)
{
	m_pVSDepthMap.Attach(
		StaticGraphics::LoadVertexShaderFile(g_pd3dDevice, szFileName));
	return true;
}

bool myDepthMap::CreateDepthPS(const TCHAR * szFileName)
{
	m_pPSDepthMap.Attach(
		StaticGraphics::LoadPixelShaderFile(g_pd3dDevice, szFileName));
	return true;
}

HRESULT myDepthMap::CreateShadowCB()
{
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(cbDepthMap);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = &m_cbData;
	hr = g_pd3dDevice->CreateBuffer(&bd, &data, m_pCBDepthMap.GetAddressOf());
	if (FAILED(hr)) return hr;
	return hr;
}
