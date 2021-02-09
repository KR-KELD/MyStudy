#pragma once
#include "myStd.h"
class myDxState
{
public:
	static ID3D11RasterizerState*		m_pRSSolidBack;
	static ID3D11RasterizerState*		m_pRSWireBack;
	static ID3D11RasterizerState*		m_pRS;
	static ID3D11SamplerState*			m_pWrapLinear;
	static ID3D11DepthStencilState*		m_pDSS;

	static D3D11_FILL_MODE			m_FillMode;
	static D3D11_CULL_MODE			m_CullMode;
public:
	static bool	Set(ID3D11Device* pd3dDevice);
	static bool	SetRasterizerState(ID3D11Device* pd3dDevice);
	static bool Release();
};

