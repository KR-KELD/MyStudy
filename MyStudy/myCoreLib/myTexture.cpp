#include "myTexture.h"

bool myTexture::Init()
{
	return true;
}

bool myTexture::Release()
{
	return true;
}

bool myTexture::Load(T_STR strTex)
{

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

}

myTexture::~myTexture()
{
}
