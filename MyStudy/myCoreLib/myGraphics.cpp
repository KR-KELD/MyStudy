#include "myGraphics.h"
DECLARE_COMPONENT(myGraphics);
void myGraphics::CompilerCheck(ID3DBlob* pErrorMsgs)
{
	C_STR szMsg = (char*)pErrorMsgs->GetBufferPointer();
	T_STR szError = to_mw(szMsg);
	MessageBox(NULL, szError.c_str(), L"ERROR", MB_OK);
}
bool	myGraphics::Init()
{
	m_matWorld = Matrix::Identity;
	m_matView = Matrix::Identity;
	m_matProj = Matrix::Identity;
	return true;
}
bool	myGraphics::Frame() 
{
	return true;
}
bool	myGraphics::SetMatrix(Matrix* pWorld,
	Matrix* pView,
	Matrix* pProj)
{
	if (pWorld != nullptr)
	{
		m_matWorld = *pWorld;
	}
	if (pView != nullptr)
	{
		m_matView = *pView;
	}
	if (pProj != nullptr)
	{
		m_matProj = *pProj;
	}
	return true;
}
bool    myGraphics::Update(ID3D11DeviceContext*	pd3dContext)
{
	D3D11_MAPPED_SUBRESOURCE mr;
	HRESULT hr = pd3dContext->Map(m_pConstantBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &mr);
	if (SUCCEEDED(hr))
	{
		myDataCB* pData = (myDataCB*)mr.pData;
		pData->matWorld = m_matWorld.Transpose();
		pData->matView = m_matView.Transpose();
		pData->matProject = m_matProj.Transpose();

		pData->vColor[0] = cosf(g_fGameTimer);
		pData->vColor[1] = sinf(g_fGameTimer);
		pData->vColor[2] = 1.0f - cosf(g_fGameTimer);
		pData->vColor[3] = 1;
		pData->vTime[0] = cosf(g_fGameTimer)*0.5f + 0.5f;
		pData->vTime[1] = g_fGameTimer;
		pd3dContext->Unmap(m_pConstantBuffer, 0);
		return true;
	}
	return false;
}
bool myGraphics::PreRender(ID3D11DeviceContext * pd3dContext)
{
	return true;
}
bool	myGraphics::Render(ID3D11DeviceContext*	pd3dContext)
{
	Update(pd3dContext);
	PreRender(pd3dContext);
	UINT iStride = sizeof(PNCT_VERTEX);
	UINT iOffset = 0;
	pd3dContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &iStride, &iOffset);
	pd3dContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pd3dContext->IASetInputLayout(m_pInputLayout);
	pd3dContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	pd3dContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	pd3dContext->VSSetShader(m_pVertexShader, NULL, 0);
	pd3dContext->PSSetShader(m_pPixelShader, NULL, 0);
	pd3dContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)m_iTopology);
	if (m_pTexture != nullptr)
	{
		pd3dContext->PSSetShaderResources(0, 1,
			&m_pTexture->m_pTextureSRV);
	}
	//pd3dContext->Draw(m_VertexList.size(), 0);
	PostRender(pd3dContext);
	return true;
}
bool myGraphics::PostRender(ID3D11DeviceContext * pd3dContext)
{
	pd3dContext->DrawIndexed(m_IndexList.size(), 0, 0);
	return true;
}
bool	myGraphics::Release()
{
	m_pConstantBuffer->Release();
	m_pVertexBuffer->Release();
	m_pIndexBuffer->Release();
	m_pInputLayout->Release();
	m_pVertexShader->Release();
	m_pPixelShader->Release();
	return true;
}

bool myGraphics::CreateVertexData(Vector3 & vCenter, float & fRange)
{
	return true;
}

myGraphics::myGraphics()
{
	m_szVertexShader = "VS";
	m_szPixelShader = "PS";
	m_iTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}
myGraphics::~myGraphics()
{

}
bool    myGraphics::CreateVertexData() 
{
	return true;
};

bool    myGraphics::CreateIndexData() 
{
	return true;
};
bool    myGraphics::CreateConstantBuffer()
{
	// constant buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(myDataCB);
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_cbData;
	HRESULT hr = m_pd3dDevice->CreateBuffer(&bd, NULL, &m_pConstantBuffer);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
bool	myGraphics::CreateVertexBuffer()
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(PNCT_VERTEX) * m_VertexList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_VertexList.at(0);
	HRESULT hr = m_pd3dDevice->CreateBuffer(&bd, &sd, &m_pVertexBuffer);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
bool	myGraphics::CreateIndexBuffer()
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(DWORD) * m_IndexList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_IndexList.at(0);
	HRESULT hr = m_pd3dDevice->CreateBuffer(&bd, &sd, &m_pIndexBuffer);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
bool myGraphics::LoadShader(T_STR szVS, T_STR szPS)
{
	ID3DBlob* pPSObj;
	ID3DBlob* pErrorMsgs;
	HRESULT hr = D3DCompileFromFile(szVS.c_str(), NULL, NULL,
		m_szVertexShader.c_str(), "vs_5_0", 0, 0, &m_pVSObj, &pErrorMsgs);
	if (FAILED(hr))
	{
		CompilerCheck(pErrorMsgs);
		return false;
	}
	hr = m_pd3dDevice->CreateVertexShader(m_pVSObj->GetBufferPointer(), m_pVSObj->GetBufferSize(), NULL, &m_pVertexShader);
	if (FAILED(hr)) return false;

	hr = D3DCompileFromFile(szPS.c_str(), NULL, NULL,
		m_szPixelShader.c_str(), "ps_5_0", 0, 0, &pPSObj, &pErrorMsgs);
	if (FAILED(hr))
	{
		CompilerCheck(pErrorMsgs);
		return false;
	}
	hr = m_pd3dDevice->CreatePixelShader(pPSObj->GetBufferPointer(), pPSObj->GetBufferSize(), NULL, &m_pPixelShader);
	if (FAILED(hr)) return false;

	if (pPSObj)	pPSObj->Release();
	return true;
}
bool	myGraphics::LoadTexture(T_STR szTex)
{
	m_pTexture = g_TextureMgr.Load(m_pd3dDevice, szTex.c_str());
	if (m_pTexture == nullptr) return false;
	return true;
}
bool	myGraphics::CreateInputLayout()
{
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE",  0, DXGI_FORMAT_R32G32_FLOAT, 0, 40,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT iNumElement = sizeof(layout) / sizeof(layout[0]);
	HRESULT hr = m_pd3dDevice->CreateInputLayout(
		layout,
		iNumElement,
		m_pVSObj->GetBufferPointer(),
		m_pVSObj->GetBufferSize(),
		&m_pInputLayout
	);
	if (FAILED(hr)) return false;
	return true;
}
bool	myGraphics::Create(ID3D11Device* pDevice, T_STR szVS, T_STR szPS, T_STR	szTex)
{
	m_pd3dDevice = pDevice;

	CreateVertexData();
	CreateConstantBuffer();
	CreateVertexBuffer();
	CreateIndexData();
	CreateIndexBuffer();
	LoadShader(szVS, szPS);
	CreateInputLayout();
	LoadTexture(szTex);
	return true;
}

