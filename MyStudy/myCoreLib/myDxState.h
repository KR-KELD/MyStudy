#pragma once
#include "myStd.h"
class myDxState
{
public:
	static ComPtr<ID3D11RasterizerState>		m_pRSSolidBack;
	static ComPtr<ID3D11RasterizerState>		m_pRSWireBack;
	static ComPtr<ID3D11RasterizerState>		m_pRS;
	static ComPtr<ID3D11SamplerState>			m_pWrapLinear;
	static ComPtr<ID3D11DepthStencilState>		m_pDSS;

	static D3D11_FILL_MODE			m_FillMode;
	static D3D11_CULL_MODE			m_CullMode;
public:
	static bool	Set();
	static bool	SetRasterizerState();
	static bool Release();
};

