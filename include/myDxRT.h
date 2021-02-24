#pragma once
#include "myStd.h"
class myDxRT
{
public:
	ID3D11DeviceContext*	m_pd3dContext;
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
	virtual void		Set(ID3D11DeviceContext* pd3dContext);
	virtual bool		SetViewport();
	virtual HRESULT		SetRenderTargetView();
	virtual HRESULT		SetDepthStencilView();
	virtual bool		Begin();
	virtual bool		End();
	virtual void		ClearShaderResources();
	virtual bool		Release();
};

