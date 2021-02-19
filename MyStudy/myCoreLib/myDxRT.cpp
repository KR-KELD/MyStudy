#include "myDxRT.h"
void		myDxRT::Set(ID3D11Device*	pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;
}
HRESULT		myDxRT::SetRenderTargetView()
{
	HRESULT hr;
	ID3D11Texture2D* pTexture = nullptr;
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
	hr = m_pd3dDevice->CreateTexture2D(&texDesc, NULL, &pTexture);
	if (FAILED(hr))
	{
		return false;
	}

	hr = m_pd3dDevice->CreateRenderTargetView(
		pTexture, NULL,
		&m_pRTV);

	hr = m_pd3dDevice->CreateShaderResourceView(pTexture,
		NULL,
		&m_pSRV);

	if (pTexture) pTexture->Release();
	return hr;
}
bool  myDxRT::Release()
{
	m_pSRV->Release();
	m_pRTV->Release();
	m_pDSV->Release();
	return true;
}
HRESULT myDxRT::SetDepthStencilView()
{
	// create depth texture
	ID3D11Texture2D* pTexture = nullptr;
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
	HRESULT hr = m_pd3dDevice->CreateTexture2D(&texDesc, NULL, &pTexture);
	if (FAILED(hr))
	{
		return false;
	}
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	hr = m_pd3dDevice->CreateDepthStencilView(
		pTexture,
		&dsvDesc,
		&m_pDSV);

	if (pTexture)pTexture->Release();
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
void myDxRT::ClearShaderResources(ID3D11DeviceContext*  pImmediateContext)
{
	// unbind resources
	ID3D11ShaderResourceView* pNULLViews[8] = { NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL };
	pImmediateContext->VSSetShaderResources(0, 8, pNULLViews);
	pImmediateContext->GSSetShaderResources(0, 8, pNULLViews);
	pImmediateContext->PSSetShaderResources(0, 8, pNULLViews);
	pImmediateContext->PSSetShaderResources(1, 8, pNULLViews);
	pImmediateContext->PSSetShaderResources(2, 8, pNULLViews);
	pImmediateContext->PSSetShaderResources(3, 8, pNULLViews);
}
bool myDxRT::Begin(ID3D11DeviceContext*	pd3dContext)
{
	UINT  iNumView = 1;
	pd3dContext->RSGetViewports(&iNumView, &vpSave);
	pd3dContext->OMGetRenderTargets(1, &pSaveRTV, &pSaveDSV);

	ID3D11RenderTargetView* pNull = NULL;
	pd3dContext->OMSetRenderTargets(1, &pNull, NULL);
	ClearShaderResources(pd3dContext);

	// apply
	pd3dContext->RSSetViewports(1, &m_ViewPort);
	pd3dContext->OMSetRenderTargets(1, &m_pRTV, m_pDSV);
	float clearColor[] = { 0,0,0,1 };
	pd3dContext->ClearRenderTargetView(m_pRTV, clearColor);
	pd3dContext->ClearDepthStencilView(m_pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	return true;
}
bool myDxRT::End(ID3D11DeviceContext*	pd3dContext)
{
	pd3dContext->OMSetRenderTargets(1, &pSaveRTV, pSaveDSV);
	pd3dContext->RSSetViewports(iNumView, &vpSave);

	pSaveRTV->Release();
	pSaveDSV->Release();
	return true;
}
