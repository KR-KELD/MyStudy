#include "myModelGraphics.h"
DECLARE_COMPONENT(myModelGraphics);

bool myModelGraphics::CreateInputLayout()
{
	HRESULT hr = S_OK;
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },

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

bool myModelGraphics::MultiDraw()
{
	g_pImmediateContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	g_pImmediateContext->IASetInputLayout(m_pInputLayout.Get());
	g_pImmediateContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
	g_pImmediateContext->PSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
	g_pImmediateContext->VSSetShader(m_pVertexShader.Get(), NULL, 0);
	g_pImmediateContext->PSSetShader(m_pPixelShader.Get(), NULL, 0);
	g_pImmediateContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)m_iTopology);

	for (int iSub = 0; iSub < m_SubMeshList.size(); iSub++)
	{
		mySubMesh* pMesh = &m_SubMeshList[iSub];
		if (pMesh->m_TriangleList.size() <= 0) continue;

		ID3D11Buffer* vb[2] = { pMesh->m_pVertexBuffer.Get(), pMesh->m_pVertexBufferIW.Get() };
		UINT iStride[2] = { sizeof(PNCT_VERTEX) ,sizeof(IW_VERTEX) };
		UINT iOffset[2] = { 0,0 };
		g_pImmediateContext->IASetVertexBuffers(0, 2, vb, iStride, iOffset);

		if (m_SubMeshList[iSub].m_pTexture != nullptr)
		{
			g_pImmediateContext->PSSetShaderResources(0, 1,
				m_SubMeshList[iSub].m_pTexture->m_pTextureSRV.GetAddressOf());
		}
		if (m_pIndexBuffer.Get() == nullptr)
		{
			g_pImmediateContext->Draw(m_SubMeshList[iSub].m_VertexList.size(), 0);
		}
		else
		{
			g_pImmediateContext->DrawIndexed(m_IndexList.size(), 0, 0);
		}
	}
	return true;
}

myModelGraphics::myModelGraphics()
{
}

myModelGraphics::~myModelGraphics()
{
}
