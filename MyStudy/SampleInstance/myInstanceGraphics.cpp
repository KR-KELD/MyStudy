#include "myInstanceGraphics.h"
DECLARE_COMPONENT(myInstanceGraphics);
bool myInstanceGraphics::CreateVertexData(Vector3 vCenter, float fRange)
{
	m_pTransform->m_vPos = vCenter;
	m_fRange = fRange;
	m_VertexList.resize(24);
	m_VertexList[0] = { Vector3(-m_fRange + vCenter.x, m_fRange + vCenter.y, -m_fRange + vCenter.z),
						Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f) };
	m_VertexList[1] = { Vector3(m_fRange + vCenter.x, m_fRange + vCenter.y, -m_fRange + vCenter.z),
						Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f) };
	m_VertexList[2] = { Vector3(-m_fRange + vCenter.x,-m_fRange + vCenter.y, -m_fRange + vCenter.z),
						Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f) };
	m_VertexList[3] = { Vector3(m_fRange + vCenter.x,-m_fRange + vCenter.y, -m_fRange + vCenter.z),
						Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f) };

	m_VertexList[4] = { Vector3(m_fRange + vCenter.x, m_fRange + vCenter.y, m_fRange + vCenter.z),
						Vector3(0.0f, 0.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f) };
	m_VertexList[5] = { Vector3(-m_fRange + vCenter.x, m_fRange + vCenter.y, m_fRange + vCenter.z),
						Vector3(0.0f, 0.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f) };
	m_VertexList[6] = { Vector3(m_fRange + vCenter.x,-m_fRange + vCenter.y, m_fRange + vCenter.z),
						Vector3(0.0f, 0.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f) };
	m_VertexList[7] = { Vector3(-m_fRange + vCenter.x,-m_fRange + vCenter.y, m_fRange + vCenter.z),
						Vector3(0.0f, 0.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f) };

	m_VertexList[8] = { Vector3(-m_fRange + vCenter.x, m_fRange + vCenter.y, m_fRange + vCenter.z),
						Vector3(-1.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f) };
	m_VertexList[9] = { Vector3(-m_fRange + vCenter.x, m_fRange + vCenter.y,-m_fRange + vCenter.z),
						Vector3(-1.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f) };
	m_VertexList[10] = { Vector3(-m_fRange + vCenter.x,-m_fRange + vCenter.y, m_fRange + vCenter.z),
						Vector3(-1.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f) };
	m_VertexList[11] = { Vector3(-m_fRange + vCenter.x,-m_fRange + vCenter.y,-m_fRange + vCenter.z),
						Vector3(-1.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f) };

	m_VertexList[12] = { Vector3(m_fRange + vCenter.x, m_fRange + vCenter.y,-m_fRange + vCenter.z),
						Vector3(1.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f) };
	m_VertexList[13] = { Vector3(m_fRange + vCenter.x, m_fRange + vCenter.y, m_fRange + vCenter.z),
						Vector3(1.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f) };
	m_VertexList[14] = { Vector3(m_fRange + vCenter.x,-m_fRange + vCenter.y,-m_fRange + vCenter.z),
						Vector3(1.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f) };
	m_VertexList[15] = { Vector3(m_fRange + vCenter.x,-m_fRange + vCenter.y, m_fRange + vCenter.z),
						Vector3(1.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f) };

	m_VertexList[16] = { Vector3(-m_fRange + vCenter.x, m_fRange + vCenter.y, m_fRange + vCenter.z),
						Vector3(0.0f, 1.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f) };
	m_VertexList[17] = { Vector3(m_fRange + vCenter.x, m_fRange + vCenter.y, m_fRange + vCenter.z),
						Vector3(0.0f, 1.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f) };
	m_VertexList[18] = { Vector3(-m_fRange + vCenter.x, m_fRange + vCenter.y,-m_fRange + vCenter.z),
						Vector3(0.0f, 1.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f) };
	m_VertexList[19] = { Vector3(m_fRange + vCenter.x, m_fRange + vCenter.y,-m_fRange + vCenter.z),
						Vector3(0.0f, 1.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f) };

	m_VertexList[20] = { Vector3(-m_fRange + vCenter.x,-m_fRange + vCenter.y,-m_fRange + vCenter.z),
						Vector3(0.0f, -1.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f) };
	m_VertexList[21] = { Vector3(m_fRange + vCenter.x,-m_fRange + vCenter.y,-m_fRange + vCenter.z),
						Vector3(0.0f, -1.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f) };
	m_VertexList[22] = { Vector3(-m_fRange + vCenter.x,-m_fRange + vCenter.y, m_fRange + vCenter.z),
						Vector3(0.0f, -1.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f) };
	m_VertexList[23] = { Vector3(m_fRange + vCenter.x,-m_fRange + vCenter.y, m_fRange + vCenter.z),
						Vector3(0.0f, -1.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f) };

	m_iNumVertex = m_VertexList.size();
	m_iVertexSize = sizeof(PNCT_VERTEX);
	return true;
}

bool myInstanceGraphics::CreateVertexData()
{
	Vector3 vCenter = { 0.0f, 0.0f ,0.0f };
	float fRange = 1.0f;
	CreateVertexData(vCenter, fRange);
	return true;
}

bool myInstanceGraphics::CreateIndexData()
{
	m_IndexList.resize(36);
	for (int i = 0; i < 6; i++)
	{
		m_IndexList[0 + i * 6] = 0 + i * 4;
		m_IndexList[1 + i * 6] = 1 + i * 4;
		m_IndexList[2 + i * 6] = 2 + i * 4;
		m_IndexList[3 + i * 6] = 2 + i * 4;
		m_IndexList[4 + i * 6] = 1 + i * 4;
		m_IndexList[5 + i * 6] = 3 + i * 4;
	}
	m_iNumIndex = m_IndexList.size();
	return true;
}

bool myInstanceGraphics::CreateInputLayout()
{
	//ÅºÁ¨Æ®Ãß°¡
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE",  0, DXGI_FORMAT_R32G32_FLOAT, 0, 40,  D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{"mTransform", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,1,0,D3D11_INPUT_PER_INSTANCE_DATA,1},
		{"mTransform", 1, DXGI_FORMAT_R32G32B32A32_FLOAT,1,16,D3D11_INPUT_PER_INSTANCE_DATA,1},
		{"mTransform", 2, DXGI_FORMAT_R32G32B32A32_FLOAT,1,32,D3D11_INPUT_PER_INSTANCE_DATA,1},
		{"mTransform", 3, DXGI_FORMAT_R32G32B32A32_FLOAT,1,48,D3D11_INPUT_PER_INSTANCE_DATA,1},
	};
	UINT iNumElement = sizeof(layout) / sizeof(layout[0]);
	HRESULT hr = g_pd3dDevice->CreateInputLayout(
		layout,
		iNumElement,
		m_pVSObj->GetBufferPointer(),
		m_pVSObj->GetBufferSize(),
		m_pInputLayout.GetAddressOf()
	);
	if (FAILED(hr)) return false;
	return true;
}

bool myInstanceGraphics::CreateInstanceBuffer()
{
	ID3D11Buffer* isb = StaticGraphics::CreateVertexBuffer(g_pd3dDevice,
		&m_InstanceList.at(0), m_InstanceList.size(), sizeof(myBaseInstance),true);
	m_pInstanceBuffer.Attach(isb);
	return false;
}

bool myInstanceGraphics::InstancingRender(ID3D11DeviceContext * pd3dContext, int i)
{
	ID3D11Buffer* vb[2] = { m_pVertexBuffer.Get(), m_pInstanceBuffer.Get() };
	//pnct2
	UINT iStride[2] = { m_iVertexSize ,sizeof(myBaseInstance) };
	UINT iOffset[2] = { 0,0 };

	pd3dContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	pd3dContext->IASetInputLayout(m_pInputLayout.Get());
	pd3dContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
	pd3dContext->PSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
	pd3dContext->VSSetShader(m_pVertexShader.Get(), NULL, 0);
	pd3dContext->PSSetShader(m_pPixelShader.Get(), NULL, 0);
	pd3dContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)m_iTopology);
	pd3dContext->IASetVertexBuffers(0, 2, vb, iStride, iOffset);
	if (m_pTexture != nullptr)
	{
		pd3dContext->PSSetShaderResources(0, 1,
			m_pTexture->m_pTextureSRV.GetAddressOf());
	}

	pd3dContext->DrawIndexedInstanced(m_iNumIndex, i, 0, 0, 0);
	//pd3dContext->DrawIndexed(m_iNumIndex, 0, 0);
	return false;
}

myInstanceGraphics::myInstanceGraphics()
{
	m_InstanceList.resize(m_iNumIntance);
}

myInstanceGraphics::~myInstanceGraphics()
{
}
