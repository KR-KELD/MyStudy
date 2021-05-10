#include "myGraphics.h"
DECLARE_COMPONENT(myGraphics);

#pragma region staticFunc
bool AscendingTriangle(myTriangle& a, myTriangle& b)
{
	return a.iSubIndex > b.iSubIndex;
}

bool CreateVnIFromTri(vector<PNCT_VERTEX>& vertexList, vector<DWORD>& indexList, vector<myTriangle>& triList)
{
	int iTriNum = triList.size();
	vertexList.clear();
	indexList.clear();
	vertexList.reserve(iTriNum * 3);
	indexList.reserve(iTriNum * 3);

	return true;
}
#pragma endregion

void myGraphics::CompilerCheck(ID3DBlob* pErrorMsgs)
{
	C_STR szMsg = (char*)pErrorMsgs->GetBufferPointer();
	T_STR szError = to_mw(szMsg);
	MessageBox(NULL, szError.c_str(), L"ERROR", MB_OK);
}
bool	myGraphics::Init()
{
	return true;
}
bool	myGraphics::Frame()
{
	return true;
}

void    myGraphics::Update(ID3D11DeviceContext*	pd3dContext)
{
	D3D11_MAPPED_SUBRESOURCE mr;
	HRESULT hr = pd3dContext->Map(m_pConstantBuffer.Get(), 0,
		D3D11_MAP_WRITE_DISCARD, 0, &mr);
	if (SUCCEEDED(hr))
	{
		myDataCB* pData = (myDataCB*)mr.pData;
		pData->matWorld = m_pTransform->m_matWorld.Transpose();
		pData->matView = m_pTransform->m_matView.Transpose();
		pData->matProject = m_pTransform->m_matProj.Transpose();

		pData->vColor[0] = m_cbData.vColor[0];
		pData->vColor[1] = m_cbData.vColor[1];
		pData->vColor[2] = m_cbData.vColor[2];
		pData->vColor[3] = 1;
		pData->vTime[0] = cosf(g_fGameTimer)*0.5f + 0.5f;
		pData->vTime[1] = g_fGameTimer;
		pd3dContext->Unmap(m_pConstantBuffer.Get(), 0);
	}
}
bool myGraphics::PreRender(ID3D11DeviceContext*	pd3dContext)
{
	return true;
}
bool	myGraphics::Render(ID3D11DeviceContext*	pd3dContext)
{
	Update(pd3dContext);
	PreRender(pd3dContext);
	if (m_SubMeshList.size() == 0)
	{
		SettingPipeLine(pd3dContext);
		Draw(pd3dContext);
	}
	else
	{
		MultiDraw(pd3dContext);
	}

	//pd3dContext->Draw(m_VertexList.size(), 0);

	PostRender(pd3dContext);
	return true;
}
bool myGraphics::PostRender(ID3D11DeviceContext*	pd3dContext)
{
	return true;
}
bool	myGraphics::Release()
{
	//m_pConstantBuffer->Release();
	//m_pVertexBuffer->Release();
	//m_pIndexBuffer->Release();
	//m_pInputLayout->Release();
	//m_pVertexShader->Release();
	//m_pPixelShader->Release();
	return true;
}

bool myGraphics::SettingPipeLine(ID3D11DeviceContext*	pd3dContext)
{
	UINT iStride = m_iVertexSize;
	UINT iOffset = 0;
	pd3dContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	pd3dContext->IASetInputLayout(m_pInputLayout.Get());
	pd3dContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
	pd3dContext->PSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
	pd3dContext->VSSetShader(m_pVertexShader.Get(), NULL, 0);
	pd3dContext->PSSetShader(m_pPixelShader.Get(), NULL, 0);
	pd3dContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)m_iTopology);
	pd3dContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &iStride, &iOffset);
	if (m_pTexture != nullptr)
	{
		pd3dContext->PSSetShaderResources(0, 1,
			m_pTexture->m_pTextureSRV.GetAddressOf());
	}
	return true;
}

bool myGraphics::MultiDraw(ID3D11DeviceContext*	pd3dContext)
{
	//pnct2
	UINT iStride = m_iVertexSize;
	UINT iOffset = 0;
	pd3dContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	pd3dContext->IASetInputLayout(m_pInputLayout.Get());
	pd3dContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
	pd3dContext->PSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
	pd3dContext->VSSetShader(m_pVertexShader.Get(), NULL, 0);
	pd3dContext->PSSetShader(m_pPixelShader.Get(), NULL, 0);
	pd3dContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)m_iTopology);
	for (int iSub = 0; iSub < m_SubMeshList.size(); iSub++)
	{
		if (m_SubMeshList[iSub].m_TriangleList.size() <= 0) continue;
		pd3dContext->IASetVertexBuffers(0, 1, m_SubMeshList[iSub].m_pVertexBuffer.GetAddressOf(), &iStride, &iOffset);

		if (m_SubMeshList[iSub].m_pTexture != nullptr)
		{
			pd3dContext->PSSetShaderResources(0, 1,
				m_SubMeshList[iSub].m_pTexture->m_pTextureSRV.GetAddressOf());
		}
		if (m_SubMeshList[iSub].m_pIndexBuffer.Get() == nullptr)
		{
			pd3dContext->Draw(m_SubMeshList[iSub].m_iNumVertex, 0);
		}
		else
		{
			pd3dContext->DrawIndexed(m_SubMeshList[iSub].m_iNumIndex, 0, 0);
		}
	}
	return true;
}

bool myGraphics::Draw(ID3D11DeviceContext*	pd3dContext)
{
	if (m_IndexList.size())
	{
		pd3dContext->DrawIndexed(m_iNumIndex, 0, 0);
	}
	else
	{
		pd3dContext->Draw(m_iNumVertex, 0);
		//pd3dContext->Draw(m_VertexList.size(), 0);
	}
	return true;
}

bool myGraphics::CreateVertexData(Vector3 vCenter, float fRange)
{
	return true;
}

myGraphics::myGraphics()
{
	m_iVertexSize = sizeof(PNCT_VERTEX);
	m_szVertexShader = "VS";
	m_szPixelShader = "PS";
	m_iTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_iNumVertex = 0;
	m_iNumIndex = 0;
}
myGraphics::~myGraphics()
{

}
bool    myGraphics::CreateVertexData()
{
	return true;
}


bool    myGraphics::CreateIndexData()
{
	return true;
}
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
	HRESULT hr = g_pd3dDevice->CreateBuffer(&bd, NULL, m_pConstantBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
bool	myGraphics::CreateVertexBuffer()
{
	if (m_iNumVertex <= 0) return true;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	//pnct2·Î º¯È¯
	bd.ByteWidth = m_iVertexSize * m_VertexList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_VertexList.at(0);
	HRESULT hr = g_pd3dDevice->CreateBuffer(&bd, &sd, m_pVertexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
bool	myGraphics::CreateIndexBuffer()
{
	if (m_iNumIndex <= 0) return true;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(DWORD) * m_IndexList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_IndexList.at(0);
	HRESULT hr = g_pd3dDevice->CreateBuffer(&bd, &sd, m_pIndexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
bool myGraphics::LoadShader(T_STR szVS, T_STR szPS)
{
	ComPtr<ID3DBlob> pPSObj;
	ComPtr<ID3DBlob> pErrorMsgs;
	HRESULT hr = D3DCompileFromFile(szVS.c_str(), NULL, NULL,
		m_szVertexShader.c_str(), "vs_5_0", 0, 0, &m_pVSObj, pErrorMsgs.GetAddressOf());
	if (FAILED(hr))
	{
		CompilerCheck(pErrorMsgs.Get());
		return false;
	}
	hr = g_pd3dDevice->CreateVertexShader(m_pVSObj->GetBufferPointer(), m_pVSObj->GetBufferSize(), NULL, m_pVertexShader.GetAddressOf());
	if (FAILED(hr)) return false;

	hr = D3DCompileFromFile(szPS.c_str(), NULL, NULL,
		m_szPixelShader.c_str(), "ps_5_0", 0, 0, &pPSObj, &pErrorMsgs);
	if (FAILED(hr))
	{
		CompilerCheck(pErrorMsgs.Get());
		return false;
	}
	hr = g_pd3dDevice->CreatePixelShader(pPSObj->GetBufferPointer(), pPSObj->GetBufferSize(), NULL, m_pPixelShader.GetAddressOf());
	if (FAILED(hr)) return false;

	if (pPSObj.Get())	pPSObj->Release();
	if (pErrorMsgs.Get())	pErrorMsgs->Release();
	return true;
}
bool	myGraphics::LoadTexture(T_STR szTex)
{
	m_pTexture = g_TextureMgr.Load(szTex.c_str());
	if (m_pTexture == nullptr) return false;
	return true;
}
bool	myGraphics::CreateInputLayout()
{
	//ÅºÁ¨Æ®Ãß°¡
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE",  0, DXGI_FORMAT_R32G32_FLOAT, 0, 40,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
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
bool	myGraphics::Create(T_STR szVS, T_STR szPS, T_STR szTex)
{
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

