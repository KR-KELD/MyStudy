#include "myShape.h"
void myShape::CompilerCheck(ID3DBlob* pErrorMsgs)
{
	C_STR szMsg = (char*)pErrorMsgs->GetBufferPointer();
	T_STR szError = to_mw(szMsg);
	MessageBox(NULL, szError.c_str(), L"ERROR", MB_OK);
}
bool	myShape::Init()
{
	m_matWorld = Matrix::Identity;
	m_matView = Matrix::Identity;
	m_matProj = Matrix::Identity;
	return true;
}
bool	myShape::Frame() 
{
	return true;
}
bool	myShape::SetMatrix(Matrix* pWorld, Matrix* pView,Matrix* pProj)
{
	//오브젝트당 각각 매트릭스 세팅
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
bool    myShape::Update(ID3D11DeviceContext*	pd3dContext)
{
	D3D11_MAPPED_SUBRESOURCE mr;
	//콘스턴트 버퍼 세팅
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
bool	myShape::Render(ID3D11DeviceContext*	pd3dContext)
{
	Update(pd3dContext);
	
	UINT iStride = sizeof(PNCT_VERTEX);
	UINT iOffset = 0;
	//인풋 어셈블러 세팅(버텍스버퍼,인덱스버퍼)
	pd3dContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &iStride, &iOffset);
	pd3dContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//인풋 레이아웃 세팅
	pd3dContext->IASetInputLayout(m_pInputLayout);
	//픽셀,버텍스 섀이더에 콘스턴트 버퍼 전달
	pd3dContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	pd3dContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	//픽셀,버텍스 섀이더 세팅
	pd3dContext->VSSetShader(m_pVertexShader, NULL, 0);
	pd3dContext->PSSetShader(m_pPixelShader, NULL, 0);
	//그려줄 도형단위 설정
	pd3dContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)m_iTopology);
	//픽셀 섀이더 리소스 세팅
	pd3dContext->PSSetShaderResources(0, 1, &m_pTextureSRV);
	//pd3dContext->Draw(m_VertexList.size(), 0);
	//그리기
	pd3dContext->DrawIndexed(m_IndexList.size(), 0, 0);
	return true;
}
bool	myShape::Relase()
{
	m_pTextureSRV->Release();
	m_pConstantBuffer->Release();
	m_pVertexBuffer->Release();
	m_pIndexBuffer->Release();
	m_pInputLayout->Release();
	m_pVertexShader->Release();
	m_pPixelShader->Release();
	return true;
}

myShape::myShape()
{
	//픽셀,버텍스 섀이더에 사용할 함수 이름, 그릴 도형단위 세팅
	m_szVertexShader = "VS";
	m_szPixelShader = "PS";
	m_iTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}
myShape::~myShape()
{

}
bool    myShape::CreateVertexData() 
{
	Vector3 vCenter = { 0.0f, 0.0f ,0.0f };
	float fRange = 1.0f;
	CreateVertexData(vCenter, fRange);
	return true;
}
bool myShape::CreateVertexData(Vector3& vCenter, float& fRange)
{
	return true;
}

bool    myShape::CreateIndexData()
{
	return true;
}
bool    myShape::CreateConstantBuffer()
{
	//콘스턴트 버퍼 옵션 세팅
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(myDataCB);
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//상세 정보 세팅
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
bool	myShape::CreateVertexBuffer()
{
	//버텍스 버퍼 세팅
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(PNCT_VERTEX) * m_VertexList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//상세 정보 세팅
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
bool	myShape::CreateIndexBuffer()
{
	//인덱스 버퍼 세팅
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(DWORD) * m_IndexList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//상세 정보 세팅
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
bool myShape::LoadShader(T_STR szVS, T_STR szPS)
{
	//섀이더 불러오기
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
bool	myShape::LoadTexture(T_STR szTex)
{
	//텍스쳐 로드
	ID3D11Resource* texture;
	HRESULT hr = DirectX::CreateWICTextureFromFile(
		m_pd3dDevice, szTex.c_str(),
		NULL,
		&m_pTextureSRV);
	if (FAILED(hr)) return false;
	return true;
}
bool	myShape::CreateInputLayout()
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
bool	myShape::Create(ID3D11Device* pDevice,
	T_STR szVS, T_STR szPS,
	T_STR	szTex)
{
	//도형에 필요한 정보들 생성,세팅
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

bool myShapeBox::CreateVertexData(Vector3& vCenter, float& fRange)
{
	m_vCenter = vCenter;
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

	return true;
}
bool    myShapeBox::CreateIndexData()
{
	//인덱스 데이터 세팅
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
	return true;
}
myShapeBox::myShapeBox()
{

}
myShapeBox::~myShapeBox()
{

}

bool myShapePlane::CreateVertexData(Vector3& vCenter, float& fRange)
{
	m_vCenter = vCenter;
	m_fRange = fRange;
	m_VertexList.resize(4);
	m_VertexList[0] =
	{
		Vector3(-fRange + vCenter.x, fRange + vCenter.y, vCenter.z),
		Vector3(0.0f,0.0f,-1.0f),
		Vector4(1,1,1,1),
		Vector2(0,0)
	};
	m_VertexList[1] =
	{
		Vector3(fRange + vCenter.x, fRange + vCenter.y, vCenter.z),
		Vector3(0.0f,0.0f,-1.0f),
		Vector4(1,1,1,1),
		Vector2(1,0)
	};
	m_VertexList[2] =
	{
		Vector3(-fRange + vCenter.x, -fRange + vCenter.y, vCenter.z),
		Vector3(0.0f,0.0f,-1.0f),
		Vector4(1,1,1,1),
		Vector2(0,1)
	};
	m_VertexList[3] =
	{
		Vector3(fRange + vCenter.x, -fRange + vCenter.y, vCenter.z),
		Vector3(0.0f,0.0f,-1.0f),
		Vector4(1,1,1,1),
		Vector2(1,1)
	};
	return true;
}

bool myShapePlane::CreateIndexData()
{
	m_IndexList.resize(6);
	int iIndex = 0;
	m_IndexList[iIndex++] = 0;
	m_IndexList[iIndex++] = 1;
	m_IndexList[iIndex++] = 2;
	m_IndexList[iIndex++] = 2;
	m_IndexList[iIndex++] = 1;
	m_IndexList[iIndex++] = 3;
	return true;
}

myShapePlane::myShapePlane()
{
}

myShapePlane::~myShapePlane()
{
}

bool myShapeLine::Draw(ID3D11DeviceContext* pd3dContext,
	Vector3 p, Vector3 e, Vector4 c)
{
	m_VertexList[0] = PNCT_VERTEX(p, Vector3(0.0f, 0.0f, -1.0f), c, Vector2(0.0f, 0.0f));
	m_VertexList[1] = PNCT_VERTEX(e, Vector3(0.0f, 0.0f, -1.0f), c, Vector2(1.0f, 0.0f));
	pd3dContext->UpdateSubresource(
		m_pVertexBuffer, 0, NULL, &m_VertexList.at(0), 0, 0);
	return myShape::Render(pd3dContext);
}
bool myShapeLine::CreateVertexData()
{
	// Vertex Data
	m_VertexList.resize(2);
	m_VertexList[0] = PNCT_VERTEX(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f));
	m_VertexList[1] = PNCT_VERTEX(Vector3(100.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(1.0f, 0.0f));
	return true;
}

bool myShapeLine::CreateIndexData()
{
	m_IndexList.resize(2);
	int iIndex = 0;
	m_IndexList[iIndex++] = 0;
	m_IndexList[iIndex++] = 1;
	return true;
}

myShapeLine::myShapeLine()
{
	m_szPixelShader = "PSLine";
	m_iTopology = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
}

myShapeLine::~myShapeLine()
{
}
