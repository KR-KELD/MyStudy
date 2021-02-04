#include "Sample.h"
GAMERUN;
bool Sample::Init()
{
	HRESULT hr = NULL;
	// load texture
	ID3D11Resource* texture;
	//�ؽ��� �ҷ�����
	hr = DirectX::CreateWICTextureFromFile(
		m_pd3dDevice, L"../../data/bitmap/flametank.bmp",
		NULL,
		&m_pTextureSRV);

	//hr = DirectX::CreateWICTextureFromFile(
	//	m_pd3dDevice, L"../../data/main_start_nor.png",
	//	NULL,
	//	&m_pTextureSRV);
	//���÷� �ɼ� ����(������ ����)
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	//��� Ȯ�� �Ӹʿ��� �������� ���
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//
	samplerDesc.BorderColor[0] = 1;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 1;
	//Lod�� ����� �Ӹ� �ּ� �ִ밪
	samplerDesc.MinLOD = FLT_MIN;
	samplerDesc.MaxLOD = FLT_MAX;
	//���÷� ����
	hr = m_pd3dDevice->CreateSamplerState(&samplerDesc, &m_pWrapLinear);


	// Rasterizer State
	//�׸��� ���
	m_FillMode = D3D11_FILL_SOLID;
	//�ø� ���
	m_CullMode = D3D11_CULL_BACK;
	//�����Ͷ����� ������Ʈ ����
	SetRasterizerState();

	//�����Ͷ����� �ɼ� ����
	D3D11_RASTERIZER_DESC rdesc;
	ZeroMemory(&rdesc, sizeof(D3D11_RASTERIZER_DESC));
	rdesc.FillMode = D3D11_FILL_SOLID;
	rdesc.CullMode = D3D11_CULL_BACK;
	hr = m_pd3dDevice->CreateRasterizerState(&rdesc, &m_pRSSolidBack);
	if (FAILED(hr))
	{
		return false;
	}

	ZeroMemory(&rdesc, sizeof(D3D11_RASTERIZER_DESC));
	rdesc.FillMode = D3D11_FILL_WIREFRAME;
	rdesc.CullMode = D3D11_CULL_BACK;
	hr = m_pd3dDevice->CreateRasterizerState(&rdesc, &m_pRSWireBack);
	if (FAILED(hr))
	{
		return false;
	}


	//���ؽ� ����Ʈ �����
	m_VertexList.resize(4);
	m_VertexList[0] = {
		myVertex3(-1.0f, 1.0f, 0.5f),
		myVertex3(0,0,0),
		myVertex4(1,0,0,1),
		myVertex2(0,0) };
	m_VertexList[1] = {
		myVertex3(1.0f, 1.0f, 0.5f),
		myVertex3(0,0,0),
		myVertex4(0,1,0,1),
		myVertex2(3,0) };
	m_VertexList[2] = {
		myVertex3(-1.0f, -1.0f, 0.5f),
		myVertex3(0,0,0),
		myVertex4(0,0,1,1),
		myVertex2(0,3) };
	m_VertexList[3] = {
		myVertex3(1.0f, -1.0f, 0.5f),
		myVertex3(0,0,0),
		myVertex4(1,1,1,1),
		myVertex2(3,3) };


	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	//GPU�� �Ҵ��� ���ؽ��� ũ��X����
	bd.ByteWidth = sizeof(myDataCB);
	//���� ���
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//���ؽ� ���� �÷��� ����
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;


	//
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	//�ý��� �޸��� �����ּ� �Ѱ��ֱ�
	sd.pSysMem = &m_cbData;
	//���ؽ� ���� ����
	hr = m_pd3dDevice->CreateBuffer(&bd, &sd, &m_pConstantBuffer);
	if (FAILED(hr))
	{
		return false;
	}
	//���� �ɼ� ����
	//D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	//GPU�� �Ҵ��� ���ؽ��� ũ��X����
	bd.ByteWidth = sizeof(MY_VERTEX) * m_VertexList.size();
	//
	bd.Usage = D3D11_USAGE_DEFAULT;
	//���ؽ� ���� �÷��� ����
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;


	//
	//D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	//�ý��� �޸��� �����ּ� �Ѱ��ֱ�
	sd.pSysMem = &m_VertexList.at(0);
	//���ؽ� ���� ����
	hr = m_pd3dDevice->CreateBuffer(&bd, &sd, &m_pVertexBuffer);
	if (FAILED(hr))
	{
		return false;
	}
	//�ε��� ����Ʈ �����
	m_IndexList.resize(6);
	m_IndexList[0] = 0;
	m_IndexList[1] = 1;
	m_IndexList[2] = 2;
	m_IndexList[3] = 2;
	m_IndexList[4] = 1;
	m_IndexList[5] = 3;

	//���� �ɼ� �ʱ�ȭ
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	//�ε��� ���� ������ �ѱ��
	bd.ByteWidth = sizeof(DWORD) * m_IndexList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	//�ε��� ���� �÷��� ����
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_IndexList.at(0);
	//�ε��� ���� ����
	hr = m_pd3dDevice->CreateBuffer(&bd, &sd, &m_pIndexBuffer);
	if (FAILED(hr))
	{
		return false;
	}
	//���̴�,���� ��� ����
	ID3DBlob* pVSObj;
	ID3DBlob* pPSObj;
	ID3DBlob* pErrorMsgs;
	//���Ϸκ��� ���ؽ� ���̴� �Լ��� �����´�
	hr = D3DCompileFromFile(L"vs.txt", NULL, NULL, "VS", "vs_5_0", 0, 0, &pVSObj, &pErrorMsgs);
	if (FAILED(hr))
	{
		CompilerCheck(pErrorMsgs);
		return false;
	}
	//������ �Լ��� ���ؽ� ���̴��� ����
	hr = m_pd3dDevice->CreateVertexShader(pVSObj->GetBufferPointer(), pVSObj->GetBufferSize(), NULL, &m_pVertexShader);
	//���Ϸκ��� �ȼ� ���̴� �Լ��� �����´�
	hr = D3DCompileFromFile(L"ps.txt", NULL, NULL, "PS", "ps_5_0", 0, 0, &pPSObj, &pErrorMsgs);
	if (FAILED(hr))
	{
		CompilerCheck(pErrorMsgs);
		return false;
	}
	//������ �Լ��� �ȼ� ���̴� ����
	hr = m_pd3dDevice->CreatePixelShader(pPSObj->GetBufferPointer(), pPSObj->GetBufferSize(), NULL, &m_pPixelShader);

	//���̴� �Լ��� ������ ������ �ɼ� ����
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE",  0, DXGI_FORMAT_R32G32_FLOAT, 0, 40,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT iNumElement = sizeof(layout) / sizeof(layout[0]);
	//��ǲ ���̾ƿ� ����
	hr = m_pd3dDevice->CreateInputLayout(
		layout,
		iNumElement,
		pVSObj->GetBufferPointer(),
		pVSObj->GetBufferSize(),
		&m_pInputLayout
	);
	return true;
}

bool Sample::Frame()
{
	D3D11_MAPPED_SUBRESOURCE mr;

	HRESULT hr = m_pd3dContext->
		Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr);
	if (SUCCEEDED(hr))
	{
		myDataCB* pData = (myDataCB*)mr.pData;
		pData->vColor[0] = cosf(g_fGameTimer);
		pData->vColor[1] = sinf(g_fGameTimer);
		pData->vColor[2] = 1.0f - cosf(g_fGameTimer);
		pData->vColor[3] = 1;
		pData->vTime[0] = cosf(g_fGameTimer) * 0.5f + 0.5f;
		pData->vTime[1] = g_fGameTimer;
		m_pd3dContext->Unmap(m_pConstantBuffer,0);
	}
	return true;
}

bool Sample::Render()
{
	//�ڸ� ����
	UINT iStride = sizeof(MY_VERTEX);
	//���� ��ġ
	UINT iOffset = 0;
	//IA ���ؽ� ���� ����
	m_pd3dContext->IASetVertexBuffers(0, 1,
		&m_pVertexBuffer, &iStride, &iOffset);
	//IA �ε��� ���� ����
	m_pd3dContext->IASetIndexBuffer(m_pIndexBuffer,
		DXGI_FORMAT_R32_UINT, 0);
	//IA ����
	m_pd3dContext->IASetInputLayout(m_pInputLayout);
	//�ܽ���Ʈ ���� ����
	m_pd3dContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pd3dContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	//���ؽ� ���̴� ����
	m_pd3dContext->VSSetShader(m_pVertexShader, NULL, 0);
	//�ȼ� ���̴� ����
	m_pd3dContext->PSSetShader(m_pPixelShader, NULL, 0);
	//IA�� �׷��� Ÿ�� ����
	m_pd3dContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//m_pd3dContext->Draw(m_VertexList.size(), 0);
	//�����Ͷ����� ������Ʈ ����
	m_pd3dContext->RSSetState(m_pRS);
	//�ȼ� ���̴��� �ؽ��� ����
	m_pd3dContext->PSSetShaderResources(0, 1, &m_pTextureSRV);
	//�ȼ� ���̴��� ���÷� ����(������)
	m_pd3dContext->PSSetSamplers(0, 1, &m_pWrapLinear);
	//�׸���
	m_pd3dContext->DrawIndexed(m_IndexList.size(), 0, 0);
	return true;
}

bool Sample::Release()
{
	m_pWrapLinear->Release();
	m_pTextureSRV->Release();
	m_pRS->Release();
	m_pRSSolidBack->Release();
	m_pRSWireBack->Release();
	m_pConstantBuffer->Release();
	m_pVertexBuffer->Release();
	m_pIndexBuffer->Release();
	m_pInputLayout->Release();
	m_pVertexShader->Release();
	m_pPixelShader->Release();
	return true;
}

void Sample::CompilerCheck(ID3DBlob * pErrorMsgs)
{
	//��ӿ��� �޽��� ��������
	C_STR szMsg = (char*)pErrorMsgs->GetBufferPointer();
	T_STR szError = to_mw(szMsg);
	MessageBox(NULL, szError.c_str(), L"ERROR", MB_OK);
}

void Sample::SetRasterizerState()
{
	HRESULT hr;
	// Rasterizer State
	if (m_pRS != nullptr) m_pRS->Release();
	D3D11_RASTERIZER_DESC rdesc;
	ZeroMemory(&rdesc, sizeof(D3D11_RASTERIZER_DESC));
	rdesc.FillMode = m_FillMode;
	rdesc.CullMode = m_CullMode;
	hr = m_pd3dDevice->CreateRasterizerState(&rdesc, &m_pRS);
	if (FAILED(hr))
	{
		return;
	}
}
