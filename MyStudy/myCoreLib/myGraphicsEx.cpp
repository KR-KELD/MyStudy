#include "myGraphicsEx.h"
DECLARE_COMPONENT(myGraphicsEx);

bool myGraphicsEx::CreateVertexBuffer()
{
	if (m_VertexList2.size() <= 0) return true;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(PNCT2_VERTEX) * m_VertexList2.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_VertexList2.at(0);
	HRESULT hr = g_pd3dDevice->CreateBuffer(&bd, &sd, m_pVertexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool myGraphicsEx::CreateInputLayout()
{
	HRESULT hr = S_OK;
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,	 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE",  0, DXGI_FORMAT_R32G32_FLOAT,		 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT,	 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "INDEX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT iNumElement = sizeof(layout) / sizeof(layout[0]);
	hr = g_pd3dDevice->CreateInputLayout(
		layout,
		iNumElement,
		m_pVSObj->GetBufferPointer(),
		m_pVSObj->GetBufferSize(),
		&m_pInputLayout
	);
	if (FAILED(hr)) return false;
	return true;
}

myGraphicsEx::myGraphicsEx()
{
	myModelGraphics::myModelGraphics();
}

myGraphicsEx::~myGraphicsEx()
{
}

bool myGraphicsEx::MultiDraw(ID3D11DeviceContext * pd3dContext)
{
	pd3dContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	pd3dContext->IASetInputLayout(m_pInputLayout.Get());
	pd3dContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
	pd3dContext->PSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
	pd3dContext->VSSetShader(m_pVertexShader.Get(), NULL, 0);
	pd3dContext->PSSetShader(m_pPixelShader.Get(), NULL, 0);
	pd3dContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)m_iTopology);

	for (int iSub = 0; iSub < m_SubMeshList.size(); iSub++)
	{
		mySubMesh* pMesh = &m_SubMeshList[iSub];
		if (pMesh->m_TriangleList.size() <= 0) continue;

		ID3D11Buffer* vb[2] = { pMesh->m_pVertexBuffer.Get(), pMesh->m_pVertexBufferIW.Get() };
		UINT iStride[2] = { sizeof(PNCT_VERTEX) ,sizeof(IW_VERTEX) };
		UINT iOffset[2] = { 0,0 };
		pd3dContext->IASetVertexBuffers(0, 2, vb, iStride, iOffset);

		if (m_SubMeshList[iSub].m_pTexture != nullptr)
		{
			pd3dContext->PSSetShaderResources(0, 1,
				m_SubMeshList[iSub].m_pTexture->m_pTextureSRV.GetAddressOf());
		}
		if (m_pIndexBuffer.Get() == nullptr)
		{
			pd3dContext->Draw(m_SubMeshList[iSub].m_VertexList.size(), 0);
		}
		else
		{
			pd3dContext->DrawIndexed(m_IndexList.size(), 0, 0);
		}
	}
	return true;
}
