#include "myDxRT.h"

HRESULT		myDxRT::SetRenderTargetView()
{
	HRESULT hr;
	ComPtr<ID3D11Texture2D> pTexture = nullptr;
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
	texDesc.Width = 512;
	texDesc.Height = 512;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET |
		D3D11_BIND_SHADER_RESOURCE;
	hr = g_pd3dDevice->CreateTexture2D(&texDesc, NULL, pTexture.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	hr = g_pd3dDevice->CreateRenderTargetView(
		pTexture.Get(), NULL,
		m_pRTV.GetAddressOf());

	hr = g_pd3dDevice->CreateShaderResourceView(pTexture.Get(),
		NULL,
		m_pSRV.GetAddressOf());
	return hr;
}
bool  myDxRT::Release()
{
	return true;
}
HRESULT myDxRT::SetDepthStencilView()
{
	// create depth texture
	ComPtr<ID3D11Texture2D> pTexture = nullptr;
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
	texDesc.Width = 512;
	texDesc.Height = 512;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	HRESULT hr = g_pd3dDevice->CreateTexture2D(&texDesc, NULL, pTexture.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	hr = g_pd3dDevice->CreateDepthStencilView(
		pTexture.Get(),
		&dsvDesc,
		m_pDSV.GetAddressOf());
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
	float clearColor[] = { 0,0,0,1 };
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
