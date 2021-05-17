#include "myStagingTex.h"

bool myStagingTex::CopyCreate(ID3D11Texture2D * pTex)
{
	D3D11_TEXTURE2D_DESC desc;
	if (pTex == nullptr) return false;
	pTex->GetDesc(&desc);

	HRESULT hr;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	desc.BindFlags = 0;
	desc.MiscFlags = 0;
	hr = g_pd3dDevice->CreateTexture2D(&desc, NULL, m_pStaging.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	g_pImmediateContext->CopyResource(m_pStaging.Get(), pTex);

	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	hr = g_pd3dDevice->CreateTexture2D(&desc, NULL, m_pTexture.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	g_pImmediateContext->CopyResource(m_pTexture.Get(), m_pStaging.Get());

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));
	SRVDesc.Format = desc.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = desc.MipLevels;
	hr = g_pd3dDevice->CreateShaderResourceView(m_pTexture.Get(), &SRVDesc, m_pSRV.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool myStagingTex::LoadTexture(const TCHAR * szFileName)
{
	HRESULT hr;
	size_t maxsize = 0;
	ComPtr<ID3D11Resource> pTexture;
	if (FAILED(hr = CreateWICTextureFromFileEx(g_pd3dDevice,
		szFileName,
		maxsize,
		D3D11_USAGE_STAGING,
		0,
		D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ,
		0,
		WIC_LOADER_DEFAULT,
		pTexture.GetAddressOf(), nullptr)))
	{
		if (FAILED(hr = CreateDDSTextureFromFileEx(g_pd3dDevice, szFileName, maxsize,
			D3D11_USAGE_STAGING,
			0,
			D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ,
			0,
			WIC_LOADER_DEFAULT,
			pTexture.GetAddressOf(), nullptr)))
		{
			return false;
		}
	}

	if (FAILED(pTexture->QueryInterface(__uuidof(ID3D11Texture2D), (LPVOID*)&m_pStaging)))
	{
		return false;
	}


	D3D11_TEXTURE2D_DESC desc;
	m_pStaging->GetDesc(&desc);

	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	hr = g_pd3dDevice->CreateTexture2D(&desc, NULL, m_pTexture.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	g_pImmediateContext->CopyResource(m_pTexture.Get(), m_pStaging.Get());

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));
	SRVDesc.Format = desc.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = desc.MipLevels;
	hr = g_pd3dDevice->CreateShaderResourceView(m_pTexture.Get(), &SRVDesc, m_pSRV.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool myStagingTex::Create(int iTexSize)
{
	D3D11_TEXTURE2D_DESC desc;

	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = iTexSize;
	desc.Height = iTexSize;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;


	HRESULT hr;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	desc.BindFlags = 0;
	desc.MiscFlags = 0;
	hr = g_pd3dDevice->CreateTexture2D(&desc, NULL, m_pStaging.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	hr = g_pd3dDevice->CreateTexture2D(&desc, NULL, m_pTexture.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));
	SRVDesc.Format = desc.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = desc.MipLevels;
	hr = g_pd3dDevice->CreateShaderResourceView(m_pTexture.Get(), &SRVDesc, m_pSRV.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
