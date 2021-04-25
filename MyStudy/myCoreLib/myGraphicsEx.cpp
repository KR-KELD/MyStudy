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

//시계방향으로 돌리면서 각 정점의 탄젠트를 채우기
//간소화하면 노말과 uv만써서 바이노말만 구해도 됨
//012면 012 120 201 순
void  myGraphicsEx::CreateTangent(PNCT2_VERTEX *v1,
	PNCT2_VERTEX *v2,
	PNCT2_VERTEX *v3,
	Vector3 *vTangent, Vector3 *vBiNormal, Vector3 *vNormal)
{
	// Create edge vectors from vertex 1 to vectors 2 and 3.
	Vector3 vEdge1 = v2->p - v1->p;
	Vector3 vEdge2 = v3->p - v1->p;
	// Create edge vectors from the texture coordinates of vertex 1 to vector 2.
	float fEdge1_U = v2->t.x - v1->t.x;
	float fEdge1_V = v2->t.y - v1->t.y;
	// Create edge vectors from the texture coordinates of vertex 1 to vector 3.
	float fEdge2_U = v3->t.x - v1->t.x;
	float fEdge2_V = v3->t.y - v1->t.y;
	float fDenominator = fEdge1_U * fEdge2_V -
		fEdge2_U * fEdge1_V;
	Vector3 vTangent, vBiNormal, vNormal;
	if (fDenominator < 0.0001f && fDenominator > -0.0001f)
	{
		*vTangent = Vector3(1.0f, 0.0f, 0.0f);
		*vBiNormal = Vector3(0.0f, 1.0f, 0.0f);
		*vNormal = Vector3(0.0f, 0.0f, 1.0f);
	}
	else
	{
		// Calculate and cache the reciprocal value
		float fScale1 = 1.0f / fDenominator;

		*vTangent = Vector3((fEdge2_V * vEdge1.x - fEdge1_V * vEdge2.x) * fScale1,
		(fEdge2_V * vEdge1.y - fEdge1_V * vEdge2.y) * fScale1,
			(fEdge2_V * vEdge1.z - fEdge1_V * vEdge2.z) * fScale1);
		*vBiNormal = Vector3((-fEdge2_U * vEdge1.x + fEdge1_U * vEdge2.x) * fScale1,
			(-fEdge2_U * vEdge1.y + fEdge1_U * vEdge2.y) * fScale1,
			(-fEdge2_U * vEdge1.z + fEdge1_U * vEdge2.z) * fScale1);
		// The normal N is calculated as the cross product between T and B
		*vNormal = vTangent->Cross(*vBiNormal);
	}
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
