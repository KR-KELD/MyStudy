#include "myHeightMap.h"
DECLARE_COMPONENT(myHeightMap);

bool myHeightMap::CreateHeightMap(const TCHAR* pszFileName, myMapDesc& mapDesc)
{
	HRESULT hr;
	ComPtr<ID3D11Resource> pTexture;
	size_t maxsize = 0;

	if (FAILED(hr = CreateWICTextureFromFileEx(g_pd3dDevice,
		pszFileName,
		maxsize,
		D3D11_USAGE_STAGING,
		0,
		D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ,
		0,
		WIC_LOADER_DEFAULT,
		pTexture.GetAddressOf(), nullptr)))
	{
		return false;
	}
	ComPtr<ID3D11Texture2D> pTexture2D = NULL;
	if (FAILED(pTexture->QueryInterface(__uuidof(ID3D11Texture2D), (LPVOID*)&pTexture2D)))
	{
		return false;
	}

	D3D11_TEXTURE2D_DESC desc;
	pTexture2D->GetDesc(&desc);

	m_fHeightList.resize(
		desc.Height*desc.Width);

	if (pTexture2D.Get())
	{
		D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
		if (SUCCEEDED(g_pImmediateContext->Map((ID3D11Resource*)pTexture2D.Get(), D3D11CalcSubresource(0, 0, 1), D3D11_MAP_READ, 0, &MappedFaceDest)))
		{
			UCHAR* pTexels = (UCHAR*)MappedFaceDest.pData;
			PNCT_VERTEX	v;
			for (UINT row = 0; row < desc.Height; row++)
			{
				UINT rowStart = row * MappedFaceDest.RowPitch;
				for (UINT col = 0; col < desc.Width; col++)
				{
					UINT colStart = col * 4;
					UINT uRed = pTexels[rowStart + colStart + 0];
					m_fHeightList[row * desc.Width + col] = uRed;	/// DWORDÀÌ¹Ç·Î pitch/4	
				}
			}
			g_pImmediateContext->Unmap(pTexture2D.Get(), D3D11CalcSubresource(0, 0, 1));
		}
	}

	m_iNumRows = desc.Height;
	m_iNumCols = desc.Width;
	return true;
}
float   myHeightMap::GetFaceHeight(UINT index)
{
	//return 0;
	return m_fHeightList[index] / m_MapDesc.fScaleHeight;
}
