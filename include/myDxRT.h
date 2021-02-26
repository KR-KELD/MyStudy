#pragma once
#include "myStd.h"
class myDxRT
{
public:
	ComPtr<ID3D11RenderTargetView>	 m_pRTV;
	ComPtr<ID3D11DepthStencilView>	 m_pDSV;
	ComPtr<ID3D11ShaderResourceView> m_pSRV;
	D3D11_VIEWPORT					 m_ViewPort;

	ID3D11RenderTargetView*			 pSaveRTV;
	ID3D11DepthStencilView*			 pSaveDSV;
	D3D11_VIEWPORT					 vpSave;
	UINT							 iNumView = 1;
public:
	virtual bool		SetViewport();
	virtual HRESULT		SetRenderTargetView();
	virtual HRESULT		SetDepthStencilView();
	virtual bool		Begin();
	virtual bool		End();
	virtual void		ClearShaderResources();
	virtual bool		Release();
};

