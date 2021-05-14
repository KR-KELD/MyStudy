#include "myStagingTex.h"

void myStagingTex::SetDesc(int iTexSize)
{
	ZeroMemory(&m_texDesc, sizeof(D3D11_TEXTURE2D_DESC));
	m_texDesc.Width = iTexSize;
	m_texDesc.Height = iTexSize;
	m_texDesc.MipLevels = 1;
	m_texDesc.ArraySize = 1;
	m_texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_texDesc.SampleDesc.Count = 1;
	m_texDesc.SampleDesc.Quality = 0;
	m_texDesc.Usage = D3D11_USAGE_STAGING;
	m_texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	m_texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	m_texDesc.BindFlags = 0;
	m_texDesc.MiscFlags = 0;
}

bool myStagingTex::Create(D3D11_TEXTURE2D_DESC * pDesc)
{
	D3D11_TEXTURE2D_DESC desc;
	if (pDesc == nullptr)
	{
		desc = m_texDesc;
	}
	else
	{
		desc = *pDesc;
	}
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
