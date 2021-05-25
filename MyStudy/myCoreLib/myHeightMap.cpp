#include "myHeightMap.h"
DECLARE_COMPONENT(myHeightMap)

bool myHeightMap::CreateHeightMap(const TCHAR* pszFileName, myMapDesc& mapDesc, myMapCB& tileInfo)
{
	m_cbMapData = tileInfo;
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
		if (FAILED(hr = CreateDDSTextureFromFileEx(g_pd3dDevice, pszFileName, maxsize,
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
	ComPtr<ID3D11Texture2D> pTexture2D = NULL;
	if (FAILED(pTexture->QueryInterface(__uuidof(ID3D11Texture2D), (LPVOID*)&pTexture2D)))
	{
		return false;
	}

	D3D11_TEXTURE2D_DESC desc;
	pTexture2D->GetDesc(&desc);

	//m_fHeightList.resize(
	//	desc.Height*desc.Width);
	m_fHeightList.resize(mapDesc.iNumCols * mapDesc.iNumRows);
	if (pTexture2D.Get())
	{
		D3D11_MAPPED_SUBRESOURCE MappedFaceDest;

		float fWidthRatio = mapDesc.iNumCols / (float)desc.Width;
		float fHeightRatio = mapDesc.iNumRows / (float)desc.Height;

		if (SUCCEEDED(g_pImmediateContext->Map((ID3D11Resource*)pTexture2D.Get(), D3D11CalcSubresource(0, 0, 1), D3D11_MAP_READ, 0, &MappedFaceDest)))
		{
			UCHAR* pTexels = (UCHAR*)MappedFaceDest.pData;
			PNCT_VERTEX	v;
			for (UINT row = 0; row < desc.Height; row++)
			{
				UINT rowStart = row * MappedFaceDest.RowPitch;
				int iRow = row * fHeightRatio;
				for (UINT col = 0; col < desc.Width; col++)
				{
					int iCol = col * fWidthRatio;
					UINT colStart = col * 4;
					UINT uRed = pTexels[rowStart + colStart + 0];
					//m_fHeightList[row * desc.Width + col] = uRed;	/// DWORD이므로 pitch/4
					m_fHeightList[iRow * mapDesc.iNumRows + iCol] = uRed;
				}
			}
			g_pImmediateContext->Unmap(pTexture2D.Get(), D3D11CalcSubresource(0, 0, 1));
		}
	}

	//m_iNumRows = desc.Height;
	//m_iNumCols = desc.Width;
	CreateMap(mapDesc);
	return true;
}
float   myHeightMap::GetFaceHeight(UINT index)
{
	//return 0;
	if (m_fHeightList.size())
	{
		return m_fHeightList[index] / m_MapDesc.fScaleHeight;
	}
	return 0;
}

bool myHeightMap::CreateHeightMap(ID3D11Texture2D * pTex, myMapDesc & mapDesc, myMapCB & tileInfo)
{
	HRESULT hr = S_OK;
	m_cbMapData = tileInfo;

	D3D11_TEXTURE2D_DESC desc;
	pTex->GetDesc(&desc);

	m_fHeightList.resize(mapDesc.iNumCols * mapDesc.iNumRows);
	if (pTex != nullptr)
	{
		D3D11_MAPPED_SUBRESOURCE MappedFaceDest;

		float fWidthRatio = mapDesc.iNumCols / (float)desc.Width;
		float fHeightRatio = mapDesc.iNumRows / (float)desc.Height;

		if (SUCCEEDED(hr = g_pImmediateContext->Map((ID3D11Resource*)pTex, D3D11CalcSubresource(0, 0, 1), D3D11_MAP_READ, 0, &MappedFaceDest)))
		{
			UCHAR* pTexels = (UCHAR*)MappedFaceDest.pData;
			PNCT_VERTEX	v;
			for (UINT row = 0; row < desc.Height; row++)
			{
				UINT rowStart = row * MappedFaceDest.RowPitch;
				int iRow = row * fHeightRatio;
				for (UINT col = 0; col < desc.Width; col++)
				{
					int iCol = col * fWidthRatio;
					UINT colStart = col * 4;
					UINT uRed = pTexels[rowStart + colStart + 0];
					//m_fHeightList[row * desc.Width + col] = uRed;	/// DWORD이므로 pitch/4
					m_fHeightList[iRow * mapDesc.iNumRows + iCol] = uRed;
				}
			}
			g_pImmediateContext->Unmap(pTex, D3D11CalcSubresource(0, 0, 1));
		}
	}

	//m_iNumRows = desc.Height;
	//m_iNumCols = desc.Width;
	CreateMap(mapDesc);

	return true;
}
