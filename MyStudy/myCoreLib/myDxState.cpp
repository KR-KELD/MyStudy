#include "myDxState.h"
ComPtr<ID3D11RasterizerState>	myDxState::m_pRSSolidNone = nullptr;
ComPtr<ID3D11RasterizerState>	myDxState::m_pRSSolidBack = nullptr;
ComPtr<ID3D11RasterizerState>	myDxState::m_pRSWireBack = nullptr;
ComPtr<ID3D11RasterizerState>	myDxState::m_pRS = nullptr;
ComPtr<ID3D11SamplerState>		myDxState::m_pWrapLinear = nullptr;
ComPtr<ID3D11DepthStencilState>	myDxState::m_pDSS = nullptr;

D3D11_FILL_MODE			myDxState::m_FillMode = D3D11_FILL_SOLID;
D3D11_CULL_MODE			myDxState::m_CullMode = D3D11_CULL_NONE;

bool myDxState::Set()
{
	// DS STATE
	D3D11_DEPTH_STENCIL_DESC DepthStencilDesc;
	ZeroMemory(&DepthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	DepthStencilDesc.DepthEnable = TRUE;
	DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	HRESULT hr = g_pd3dDevice->CreateDepthStencilState(
		&DepthStencilDesc, m_pDSS.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.BorderColor[0] = 1;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 1;
	samplerDesc.MinLOD = FLT_MIN;
	samplerDesc.MaxLOD = FLT_MAX;
	hr = g_pd3dDevice->CreateSamplerState(&samplerDesc, m_pWrapLinear.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	D3D11_RASTERIZER_DESC rdesc;
	ZeroMemory(&rdesc, sizeof(D3D11_RASTERIZER_DESC));
	rdesc.FillMode = D3D11_FILL_SOLID;
	rdesc.CullMode = D3D11_CULL_BACK;
	hr = g_pd3dDevice->CreateRasterizerState(&rdesc, m_pRSSolidBack.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	rdesc.FillMode = D3D11_FILL_SOLID;
	rdesc.CullMode = D3D11_CULL_NONE;
	hr = g_pd3dDevice->CreateRasterizerState(&rdesc, m_pRSSolidNone.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	ZeroMemory(&rdesc, sizeof(D3D11_RASTERIZER_DESC));
	rdesc.FillMode = D3D11_FILL_WIREFRAME;
	rdesc.CullMode = D3D11_CULL_BACK;
	hr = g_pd3dDevice->CreateRasterizerState(&rdesc, m_pRSWireBack.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	// update
	m_FillMode = D3D11_FILL_SOLID;
	m_CullMode = D3D11_CULL_NONE;
	SetRasterizerState();
	return true;
}
bool myDxState::SetRasterizerState()
{
	HRESULT hr;
	if (m_pRS.Get() != nullptr) m_pRS->Release();
	D3D11_RASTERIZER_DESC rdesc;
	ZeroMemory(&rdesc, sizeof(D3D11_RASTERIZER_DESC));
	rdesc.FillMode = m_FillMode;
	rdesc.CullMode = m_CullMode;
	rdesc.DepthClipEnable = TRUE;
	hr = g_pd3dDevice->CreateRasterizerState(&rdesc, m_pRS.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
bool myDxState::Release()
{
	//m_pDSS->Release();
	//m_pWrapLinear->Release();
	//m_pRS->Release();
	//m_pRSSolidBack->Release();
	//m_pRSWireBack->Release();
	return true;
}