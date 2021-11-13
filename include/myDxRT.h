#pragma once
#include "myStd.h"
class myDxRT
{
public:
	ComPtr<ID3D11RenderTargetView>	 m_pRTV;
	ComPtr<ID3D11DepthStencilView>	 m_pDSV;
	ComPtr<ID3D11ShaderResourceView> m_pSRV;
	ComPtr<ID3D11ShaderResourceView> m_pSRV_DSV;
	ComPtr<ID3D11Texture2D>			 m_pTexture2D;
	//ComPtr<ID3D11Texture2D>			 m_pTexture2D_DSV;
	D3D11_VIEWPORT					 m_ViewPort;

	ID3D11RenderTargetView*			 pSaveRTV;
	ID3D11DepthStencilView*			 pSaveDSV;
	D3D11_VIEWPORT					 vpSave;
	UINT							 iNumView = 1;
public:
	D3D11_TEXTURE2D_DESC			m_TexDesc;
	D3D11_RENDER_TARGET_VIEW_DESC	m_RTVDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC m_SRVDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC	m_DSVDesc;
public:

public:
	virtual bool		SetViewport(int iWidth = 512, int iHeight = 512);
	virtual HRESULT		SetRenderTargetView(D3D11_TEXTURE2D_DESC* texDesc = nullptr);
	virtual HRESULT		SetDepthStencilView(D3D11_TEXTURE2D_DESC* texDesc = nullptr,
		D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc = nullptr);
	virtual bool		Begin();
	virtual bool		End();
	virtual void		ClearShaderResources();
	virtual bool		Release();
};

