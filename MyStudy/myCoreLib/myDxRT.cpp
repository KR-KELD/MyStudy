#include "myDxRT.h"

HRESULT		myDxRT::SetRenderTargetView(D3D11_TEXTURE2D_DESC* texDesc)
{
	HRESULT hr;
	ComPtr<ID3D11Texture2D> pTexture = nullptr;

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = 512;
	desc.Height = 512;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET |
		D3D11_BIND_SHADER_RESOURCE;
	if (texDesc == nullptr)
	{
		hr = g_pd3dDevice->CreateTexture2D(&desc, NULL, m_pTexture2D.GetAddressOf());
	}
	else
	{
		hr = g_pd3dDevice->CreateTexture2D(texDesc, NULL, m_pTexture2D.GetAddressOf());
	}
	if (FAILED(hr))
	{
		return false;
	}
	hr = g_pd3dDevice->CreateRenderTargetView(
		m_pTexture2D.Get(), NULL,
		m_pRTV.GetAddressOf());

	hr = g_pd3dDevice->CreateShaderResourceView(m_pTexture2D.Get(),
		NULL,
		m_pSRV.GetAddressOf());
	return hr;
}
bool  myDxRT::Release()
{
	return true;
}
HRESULT myDxRT::SetDepthStencilView(D3D11_TEXTURE2D_DESC* texDesc,
	D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc)
{
	// create depth texture
	HRESULT hr;
	ComPtr<ID3D11Texture2D> pTexture2D;
	D3D11_TEXTURE2D_DESC tdesc;
	ZeroMemory(&tdesc, sizeof(D3D11_TEXTURE2D_DESC));
	tdesc.Width = 512;
	tdesc.Height = 512;
	tdesc.MipLevels = 1;
	tdesc.ArraySize = 1;
	tdesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	tdesc.SampleDesc.Count = 1;
	tdesc.SampleDesc.Quality = 0;
	tdesc.Usage = D3D11_USAGE_DEFAULT;
	tdesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	if (texDesc == nullptr)
	{
		//hr = g_pd3dDevice->CreateTexture2D(&tdesc, NULL, m_pTexture2D_DSV.GetAddressOf());
		hr = g_pd3dDevice->CreateTexture2D(&tdesc, NULL, pTexture2D.GetAddressOf());
	}
	else
	{
		//hr = g_pd3dDevice->CreateTexture2D(texDesc, NULL, m_pTexture2D_DSV.GetAddressOf());
		hr = g_pd3dDevice->CreateTexture2D(texDesc, NULL, pTexture2D.GetAddressOf());
	}
	if (FAILED(hr))
	{
		return false;
	}
	D3D11_DEPTH_STENCIL_VIEW_DESC ddesc;
	ZeroMemory(&ddesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	ddesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	ddesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	ddesc.Texture2D.MipSlice = 0;
	if (dsvDesc == nullptr)
	{
		hr = g_pd3dDevice->CreateDepthStencilView(
			pTexture2D.Get(),//m_pTexture2D_DSV.Get(),
			&ddesc,
			m_pDSV.GetAddressOf());
	}
	else
	{
		hr = g_pd3dDevice->CreateDepthStencilView(
			pTexture2D.Get(),//m_pTexture2D_DSV.Get(),
			dsvDesc,
			m_pDSV.GetAddressOf());
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC sdesc;
	ZeroMemory(&sdesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	sdesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	sdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	sdesc.Texture2D.MipLevels = 1;

	hr = g_pd3dDevice->CreateShaderResourceView(pTexture2D.Get(),//m_pTexture2D_DSV.Get(),
		&sdesc,
		m_pSRV_DSV.GetAddressOf());

	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
bool myDxRT::SetViewport()
{
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;
	m_ViewPort.Width = 512;
	m_ViewPort.Height = 512;
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;
	return true;
}
void myDxRT::ClearShaderResources()
{
	// unbind resources
	ID3D11ShaderResourceView* pNULLViews[8] = { NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL };
	g_pImmediateContext->VSSetShaderResources(0, 8, pNULLViews);
	g_pImmediateContext->GSSetShaderResources(0, 8, pNULLViews);
	g_pImmediateContext->PSSetShaderResources(0, 8, pNULLViews);
	g_pImmediateContext->PSSetShaderResources(1, 8, pNULLViews);
	g_pImmediateContext->PSSetShaderResources(2, 8, pNULLViews);
	g_pImmediateContext->PSSetShaderResources(3, 8, pNULLViews);
}
bool myDxRT::Begin()
{
	UINT  iNumView = 1;
	g_pImmediateContext->RSGetViewports(&iNumView, &vpSave);
	g_pImmediateContext->OMGetRenderTargets(1, &pSaveRTV, &pSaveDSV);

	ID3D11RenderTargetView* pNull = NULL;
	g_pImmediateContext->OMSetRenderTargets(1, &pNull, NULL);
	ClearShaderResources();

	// apply
	g_pImmediateContext->RSSetViewports(1, &m_ViewPort);
	g_pImmediateContext->OMSetRenderTargets(1, m_pRTV.GetAddressOf(), m_pDSV.Get());
	float clearColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	g_pImmediateContext->ClearRenderTargetView(m_pRTV.Get(), clearColor);
	g_pImmediateContext->ClearDepthStencilView(m_pDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	return true;
}
bool myDxRT::End()
{
	g_pImmediateContext->OMSetRenderTargets(1, &pSaveRTV, pSaveDSV);
	g_pImmediateContext->RSSetViewports(iNumView, &vpSave);
	pSaveRTV->Release();
	pSaveDSV->Release();
	return true;
}
