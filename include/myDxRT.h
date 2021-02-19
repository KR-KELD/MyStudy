#pragma once
#include "myStd.h"
class myDxRT
{
	ID3D11Device*			m_pd3dDevice;
public:
	ID3D11RenderTargetView* m_pRTV;
	ID3D11DepthStencilView* m_pDSV;
	ID3D11ShaderResourceView* m_pSRV;
	D3D11_VIEWPORT			m_ViewPort;

	ID3D11RenderTargetView* pSaveRTV;
	ID3D11DepthStencilView* pSaveDSV;
	D3D11_VIEWPORT vpSave;
	UINT  iNumView = 1;
public:
	void				Set(ID3D11Device* pd3dDevice);
	virtual bool		SetViewport();
	virtual HRESULT		SetRenderTargetView();
	virtual HRESULT		SetDepthStencilView();
	virtual bool		Begin(ID3D11DeviceContext* pd3dContext);
	virtual bool		End(ID3D11DeviceContext* pd3dContext);
	virtual void		ClearShaderResources(ID3D11DeviceContext* pImmediateContext);
	virtual bool		Release();
};

