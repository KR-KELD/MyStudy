#include "myModelGraphics.h"
DECLARE_COMPONENT(myModelGraphics);

bool myModelGraphics::CreateVertexBuffer()
{
	myGraphics::CreateVertexBuffer();
	if (m_VertexIWList.size() <= 0) return true;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(IW_VERTEX) * m_VertexIWList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_VertexIWList.at(0);
	HRESULT hr = g_pd3dDevice->CreateBuffer(&bd, &sd, m_pVertexIWBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool myModelGraphics::CreateInputLayout()
{
	return false;
}

bool myModelGraphics::CreateConstantBuffer()
{
	myGraphics::CreateConstantBuffer();
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(Matrix) * 255; 
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_cbData;
	HRESULT hr = g_pd3dDevice->CreateBuffer(&bd, NULL, m_pConstantBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

myModelGraphics::myModelGraphics()
{
}

myModelGraphics::~myModelGraphics()
{
}
