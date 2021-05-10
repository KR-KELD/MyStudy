#include "myRenderTarget.h"

bool myRenderTarget::CreateTexture2D(D3D11_TEXTURE2D_DESC* desc)
{
	HRESULT hr = S_OK;
	if (desc == nullptr)
	{
		hr = g_pd3dDevice->CreateTexture2D(&m_TexDesc, NULL, m_pTexture2D.GetAddressOf());
	}
	else
	{
		hr = g_pd3dDevice->CreateTexture2D(desc, NULL, m_pTexture2D.GetAddressOf());
	}
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool myRenderTarget::CreateRenderTargetView(D3D11_RENDER_TARGET_VIEW_DESC* desc)
{
	HRESULT hr = S_OK;
	if (desc == nullptr)
	{
		hr = g_pd3dDevice->CreateRenderTargetView(m_pTexture2D.Get(),
			&m_RTVDesc, m_pRTV.GetAddressOf());
	}
	else
	{
		hr = g_pd3dDevice->CreateRenderTargetView(m_pTexture2D.Get(),
			desc, m_pRTV.GetAddressOf());
	}
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool myRenderTarget::CreateShaderResourceView(D3D11_SHADER_RESOURCE_VIEW_DESC* desc)
{
	HRESULT hr = S_OK;
	if (desc == nullptr)
	{
		hr = g_pd3dDevice->CreateShaderResourceView(m_pTexture2D.Get(),
			&m_SRVDesc, m_pSRV.GetAddressOf());
	}
	else
	{
		hr = g_pd3dDevice->CreateShaderResourceView(m_pTexture2D.Get(),
			desc, m_pSRV.GetAddressOf());
	}
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool myRenderTarget::Set()
{
	m_TexDesc.Width = g_rtClient.right;
	m_TexDesc.Height = g_rtClient.bottom;
	m_TexDesc.MipLevels = 1;
	m_TexDesc.ArraySize = 1;
	m_TexDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	m_TexDesc.SampleDesc.Count = 1;
	m_TexDesc.SampleDesc.Quality = 0;
	m_TexDesc.Usage = D3D11_USAGE_DEFAULT;
	m_TexDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	m_TexDesc.CPUAccessFlags = 0;
	m_TexDesc.MiscFlags = 0;

	m_RTVDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	m_RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	m_RTVDesc.Texture2D.MipSlice = 0;

	m_SRVDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	m_SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	m_SRVDesc.Texture2D.MostDetailedMip = 0;
	m_SRVDesc.Texture2D.MipLevels = m_TexDesc.MipLevels;
	return true;
}

myRenderTarget::myRenderTarget()
{
	Set();
}

myRenderTarget::~myRenderTarget()
{
}
