#pragma once
#include "myDxState.h"
class myRenderTarget
{
public:
	ComPtr<ID3D11RenderTargetView>		m_pRTV;
	ComPtr<ID3D11Texture2D>				m_pTexture2D;
	ComPtr<ID3D11ShaderResourceView>	m_pSRV;
public:
	D3D11_TEXTURE2D_DESC m_TexDesc;
	D3D11_RENDER_TARGET_VIEW_DESC m_RTVDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC m_SRVDesc;
public:
	bool CreateTexture2D(D3D11_TEXTURE2D_DESC* desc = nullptr);
	bool CreateRenderTargetView(D3D11_RENDER_TARGET_VIEW_DESC* desc = nullptr);
	bool CreateShaderResourceView(D3D11_SHADER_RESOURCE_VIEW_DESC* desc = nullptr);
	bool Set();
public:
	myRenderTarget();
	virtual ~myRenderTarget();
};

