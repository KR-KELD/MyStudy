#include "myComputeShader.h"

HRESULT myComputeShader::CreateComputeShader(ID3D11Device * pDevice, LPCWSTR srcfile, LPCSTR funcName)
{
	HRESULT hr = S_OK;
	if (m_pCS.Get()) return hr;
	ID3D11ComputeShader* pCS = nullptr;
	wstring strTemp = L"../../data/shader/";
	strTemp += srcfile;
	pCS = StaticGraphics::LoadComputeShaderFile(pDevice, strTemp.c_str(), nullptr, funcName);
	m_pCS.Attach(pCS);
	return hr;
}

void myComputeShader::RunComputeShader(ID3D11DeviceContext * pContext, ID3D11ShaderResourceView ** ppSRV, int iNumSRV, UINT uX, UINT uY, UINT uZ)
{
	pContext->CSSetUnorderedAccessViews(0, 1, m_pUAV.GetAddressOf(), NULL);
	pContext->CSSetShaderResources(0, iNumSRV, ppSRV);
	pContext->CSSetShader(m_pCS.Get(), NULL, 0);

	pContext->Dispatch(uX, uY, uZ); // 32*32 = 1024

	pContext->CSSetShader(NULL, NULL, 0);
	ID3D11UnorderedAccessView* ppUAViewNULL[1] = { NULL };
	pContext->CSSetUnorderedAccessViews(0, 1, ppUAViewNULL, NULL);
	ID3D11ShaderResourceView* ppSRVNULL[3] = { NULL, NULL, NULL };
	pContext->CSSetShaderResources(0, 3, ppSRVNULL);
	ID3D11Buffer* ppCBNULL[1] = { NULL };
	pContext->CSSetConstantBuffers(0, 1, ppCBNULL);
}

HRESULT myComputeShader::CreateStructuredBuffer(ID3D11Device * pDevice, UINT uElementSize, UINT uCount, void * pInitData)
{
	HRESULT hr = S_OK;
	if (m_pStBuff.Get()) return hr;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.BindFlags = D3D11_BIND_SHADER_RESOURCE
		| D3D11_BIND_UNORDERED_ACCESS;
	bd.ByteWidth = uElementSize * uCount;
	bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bd.StructureByteStride = uElementSize;
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &pInitData;
	hr = pDevice->CreateBuffer(&bd, &initData, m_pStBuff.GetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}
	return hr;
}

HRESULT myComputeShader::CreateBufferSRV(ID3D11Device * pDevice, ID3D11Buffer * pBuffer)
{
	HRESULT hr = S_OK;
	if (m_pStBuffSRV.Get()) return hr;
	D3D11_BUFFER_DESC desc;
	pBuffer->GetDesc(&desc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	ZeroMemory(&srvd, sizeof(srvd));
	srvd.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvd.BufferEx.FirstElement = 0;
	// 반드시
	srvd.Format = DXGI_FORMAT_UNKNOWN;
	srvd.BufferEx.NumElements = desc.ByteWidth / desc.StructureByteStride;
	hr = pDevice->CreateShaderResourceView(pBuffer, &srvd, m_pStBuffSRV.GetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}
	return hr;
}

HRESULT myComputeShader::CreateTextureUAV(ID3D11Device * pDevice, void* pInitData, int iWidth, int iHeight)
{
	HRESULT hr = S_OK;
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.Width = iWidth;
	td.Height = iHeight;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE; // 필수
	td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//D3D11_SUBRESOURCE_DATA data;
	//ZeroMemory(&data, sizeof(data));
	//// d3d11_usage_staging // cpu
	//// d3d11_usage_dynamic // cpu
	//// UpdateSubresource  // gup
	//// pSysMem   // init
	//data.pSysMem = pInitData;
	//data.SysMemPitch = iWidth * 4;
	//data.SysMemSlicePitch = iWidth* iHeight*4; // 3D
	if (m_pTexture.Get() == nullptr)
	{
		hr = pDevice->CreateTexture2D(&td, NULL/*&data*/, m_pTexture.GetAddressOf());
		if (FAILED(hr))
		{
			return hr;
		}
	}
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavd;
	ZeroMemory(&uavd, sizeof(uavd));
	uavd.Format = td.Format;
	uavd.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	uavd.Texture2D.MipSlice = 0;
	if (m_pUAV.Get() == nullptr)
	{
		hr = pDevice->CreateUnorderedAccessView(m_pTexture.Get(), &uavd,
			m_pUAV.GetAddressOf());
		if (FAILED(hr))
		{
			return hr;
		}
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	ZeroMemory(&srvd, sizeof(srvd));
	srvd.Format = td.Format;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MipLevels = 1;
	if (m_pSRV.Get() == nullptr)
	{
		hr = pDevice->CreateShaderResourceView(m_pTexture.Get(), &srvd,
			m_pSRV.GetAddressOf());
		if (FAILED(hr))
		{
			return hr;
		}
	}
	// copy
	if (m_pTextureCopy.Get() == nullptr)
	{
		hr = pDevice->CreateTexture2D(&td, NULL/*&data*/,
			m_pTextureCopy.GetAddressOf());
		if (FAILED(hr))
		{
			return hr;
		}
	}

	if (m_pSRVCopy.Get() == nullptr)
	{
		hr = pDevice->CreateShaderResourceView(m_pTextureCopy.Get(),
			&srvd, m_pSRVCopy.GetAddressOf());
		if (FAILED(hr))
		{
			return hr;
		}
	}

	return hr;
}

bool myComputeShader::CreateFromTexFile(const TCHAR * szFileName)
{
	HRESULT hr;
	size_t maxsize = 0;
	ComPtr<ID3D11Resource> pTexture;

	D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	if (FAILED(hr = CreateWICTextureFromFileEx(g_pd3dDevice,
		szFileName,
		maxsize,
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE,
		0,
		0,
		WIC_LOADER_DEFAULT,
		pTexture.GetAddressOf(), m_pSRV.GetAddressOf())))
	{
		if (FAILED(hr = CreateDDSTextureFromFileEx(g_pd3dDevice, szFileName, maxsize,
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE,
			0,
			0,
			WIC_LOADER_DEFAULT,
			pTexture.GetAddressOf(), m_pSRV.GetAddressOf())))
		{
			return false;
		}
	}

	if (FAILED(pTexture->QueryInterface(__uuidof(ID3D11Texture2D), (LPVOID*)&m_pTexture)))
	{
		return false;
	}

	D3D11_TEXTURE2D_DESC desc;
	m_pTexture->GetDesc(&desc);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavd;
	ZeroMemory(&uavd, sizeof(uavd));
	uavd.Format = desc.Format;
	uavd.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	uavd.Texture2D.MipSlice = 0;
	hr = g_pd3dDevice->CreateUnorderedAccessView(m_pTexture.Get(), &uavd,
		m_pUAV.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	m_pSRV->GetDesc(&srvd);
	
	// copy
	hr = g_pd3dDevice->CreateTexture2D(&desc, NULL,
		m_pTextureCopy.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	hr = g_pd3dDevice->CreateShaderResourceView(m_pTextureCopy.Get(),
		&srvd, m_pSRVCopy.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
