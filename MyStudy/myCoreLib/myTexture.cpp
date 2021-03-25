#include "myTexture.h"

bool myTexture::Init()
{
	return true;
}

bool myTexture::Frame()
{
	return true;
}

bool myTexture::Render()
{
	return true;
}

bool myTexture::Release()
{
	//if (m_pTextureSRV)
	//{
	//	m_pTextureSRV->Release();
	//	m_pTextureSRV = nullptr;
	//}
	return true;
}

bool myTexture::Load(T_STR strTex)
{
	//ID3D11Resource* texture;
	HRESULT hr = CreateWICTextureFromFile(
		g_pd3dDevice, strTex.c_str(),
		NULL,
		m_pTextureSRV.GetAddressOf());

	if (FAILED(hr))
	{
		hr = CreateDDSTextureFromFile(g_pd3dDevice, strTex.c_str(), NULL, m_pTextureSRV.GetAddressOf());
		if (FAILED(hr)) return false;
	}
	return true;
}

myTexture::myTexture()
{
	m_pTextureSRV = nullptr;
}

myTexture::~myTexture()
{
}
